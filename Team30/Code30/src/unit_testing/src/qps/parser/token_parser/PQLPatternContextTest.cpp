#include "catch.hpp"
#include <memory>
#include <unordered_map>

#include "../../util/PQLTestTokenSequenceBuilder.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/clauses/pattern/IfPatternClause.h"
#include "qps/clauses/pattern/WhilePatternClause.h"
#include "../../SourceParserStub.h"
#include "qps/parser/token_parser/parsers/conditional_parser/pattern_clause/PQLPatternParser.h"

using std::make_unique, std::unordered_map;

template <class T>
void testPatternParsing(vector<PQLToken> inputs,
                              unordered_map<string, PQLSynonymType> synonyms) {
  SourceParserStub exprParser;
  PQLPatternParser context(&exprParser);
  QueryTokenParseState state(&inputs);
  QueryBuilder builder;

  for (auto it : synonyms) {
    builder.addSynonym(it.first, it.second);
  }
  builder.finalizeSynonymTable();
  context.parse(&state, &builder);

  auto clauses = builder.build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<T*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

template <class T>
void testPatternParsing(vector<PQLToken> inputs) {
  testPatternParsing<T>(inputs, unordered_map<string, PQLSynonymType>{
      {"a", PQL_SYN_TYPE_ASSIGN},
      {"if", PQL_SYN_TYPE_IF},
      {"w", PQL_SYN_TYPE_WHILE},
      {"v", PQL_SYN_TYPE_VARIABLE},
      {"s", PQL_SYN_TYPE_STMT},
      {"b", PQL_SYN_TYPE_CONSTANT}
  });
}

void testAssignPatternParsing(vector<PQLToken> inputs) {
  testPatternParsing<AssignPatternClause>(inputs);
}

void testIfPatternParsing(vector<PQLToken> inputs) {
  testPatternParsing<IfPatternClause>(inputs);
}

void testWhilePatternParsing(vector<PQLToken> inputs) {
  testPatternParsing<WhilePatternClause>(inputs);
}

TEST_CASE("Test PQL Assign Pattern parsing Full Match") {
  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->addToken(PQL_TOKEN_PATTERN)
                               ->synonym("a")
                               ->openBracket()
                               ->synonym("v")
                               ->comma()
                               ->ident("2")
                               ->closeBracket()
                               ->build());

  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->addToken(PQL_TOKEN_PATTERN)
                               ->synonym("a")
                               ->openBracket()
                               ->ident("x")
                               ->comma()
                               ->ident("2")
                               ->closeBracket()
                               ->build());

  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->addToken(PQL_TOKEN_PATTERN)
                               ->synonym("a")
                               ->openBracket()
                               ->ident("x")
                               ->comma()
                               ->ident("y")
                               ->closeBracket()
                               ->build());

  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->addToken(PQL_TOKEN_PATTERN)
                               ->synonym("a")
                               ->openBracket()
                               ->wildcard()
                               ->comma()
                               ->ident("y")
                               ->closeBracket()
                               ->build());
}

TEST_CASE("Test PQL Assign Pattern parsing Partial Match") {
  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->wildcard()
          ->ident("2")
          ->wildcard()
          ->closeBracket()
          ->build()
  );

  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->ident("x")
          ->comma()
          ->wildcard()
          ->ident("y")
          ->wildcard()
          ->closeBracket()
          ->build()
  );

  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->ident("y")
          ->wildcard()
          ->closeBracket()
          ->build()
  );
}

TEST_CASE("Test PQL Assign Pattern parsing Wildcard") {
  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  );

  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->ident("x")
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  );

  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  );
}

TEST_CASE("Test PQL Assign Pattern parsing Complex Pattern") {
  testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->literal("1 + 2 + x - y")
          ->closeBracket()
          ->build()
  );
}

TEST_CASE("Test PQL If Pattern parsing") {
  testIfPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("if")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  );
}

TEST_CASE("Test PQL While Pattern parsing") {
  testWhilePatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("w")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  );
}

TEST_CASE("Test PQL Pattern invalid ref") {
  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("b")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSemanticError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->synonym("z")
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSemanticError);
}

TEST_CASE("Test PQL Pattern Invalid Syntax") {
  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->synonym("b")
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->integer(1)
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->integer(1)
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->addToken(PQL_TOKEN_PATTERN)
          ->synonym("a")
          ->openBracket()
          ->wildcard()
          ->comma()
          ->ident("abc")
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);
}

TEST_CASE("Test PQL Pattern invalid (_, literal) synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_VARIABLE,
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_PROCEDURE,
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"a", type}
    };

    REQUIRE_THROWS_AS(
        testPatternParsing<AssignPatternClause>(
            make_unique<PQLTestTokenSequenceBuilder>()
                ->addToken(PQL_TOKEN_PATTERN)
                ->synonym("a")
                ->openBracket()
                ->wildcard()
                ->comma()
                ->literal("x + 1")
                ->closeBracket()
                ->build()
            , synonymMap),
        QPSParserSemanticError
    );
  }
}

TEST_CASE("Test PQL Pattern invalid (_, _) synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_VARIABLE,
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_PROCEDURE,
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"a", type}
    };

    REQUIRE_THROWS_AS(
        testPatternParsing<AssignPatternClause>(
            make_unique<PQLTestTokenSequenceBuilder>()
                ->addToken(PQL_TOKEN_PATTERN)
                ->synonym("a")
                ->openBracket()
                ->wildcard()
                ->comma()
                ->literal("x + 1")
                ->closeBracket()
                ->build()
            , synonymMap),
        QPSParserSemanticError
    );
  }
}

TEST_CASE("Test PQL Assign Pattern invalid leftArg synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_ASSIGN,
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_PROCEDURE,
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"a", PQL_SYN_TYPE_ASSIGN},
        {"v", type}
    };

    REQUIRE_THROWS_AS(
        testPatternParsing<AssignPatternClause>(
            make_unique<PQLTestTokenSequenceBuilder>()
                ->addToken(PQL_TOKEN_PATTERN)
                ->synonym("a")
                ->openBracket()
                ->synonym("v")
                ->comma()
                ->wildcard()
                ->closeBracket()
                ->build()
            , synonymMap),
        QPSParserSemanticError
    );
  }
}
