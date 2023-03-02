#include "catch.hpp"
#include <memory>
#include <unordered_map>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/clauses/pattern/AssignPatternClause.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/token_parser/context/pattern_clause/PQLPatternContext.h"

using std::make_unique, std::unordered_map;

void testAssignPatternParsing(vector<PQLToken> inputs,
                              unordered_map<string, PQLSynonymType> synonyms) {
  PQLPatternContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  for (auto it : synonyms) {
    state.getQueryBuilder()->addSynonym(it.first, it.second);
  }
  context.parse(&state);
  state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<AssignPatternClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

void testAssignPatternParsing(vector<PQLToken> inputs) {
  testAssignPatternParsing(inputs, unordered_map<string, PQLSynonymType>{
      {"a", PQL_SYN_TYPE_ASSIGN},
      {"v", PQL_SYN_TYPE_VARIABLE},
      {"s", PQL_SYN_TYPE_STMT},
      {"b", PQL_SYN_TYPE_CONSTANT}
  });
}

TEST_CASE("Test PQL Assign Pattern parsing Full Match") {
  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->synonym("a")
                               ->openBracket()
                               ->synonym("v")
                               ->comma()
                               ->ident("2")
                               ->closeBracket()
                               ->build());

  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->synonym("a")
                               ->openBracket()
                               ->ident("x")
                               ->comma()
                               ->ident("2")
                               ->closeBracket()
                               ->build());

  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
                               ->synonym("a")
                               ->openBracket()
                               ->ident("x")
                               ->comma()
                               ->ident("y")
                               ->closeBracket()
                               ->build());

  testAssignPatternParsing(make_unique<PQLTestTokenSequenceBuilder>()
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
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->literal("1 + 2 + x - y")
          ->closeBracket()
          ->build()
  );
}

TEST_CASE("Test PQL Assign Pattern invalid ref") {
  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
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
          ->synonym("a")
          ->openBracket()
          ->synonym("z")
          ->comma()
          ->wildcard()
          ->closeBracket()
          ->build()
  ), QPSParserSemanticError);

  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->synonym("b")
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);
}

TEST_CASE("Test PQL Assign Pattern bad syntax") {
  REQUIRE_THROWS_AS(testAssignPatternParsing(
      make_unique<PQLTestTokenSequenceBuilder>()
          ->synonym("a")
          ->openBracket()
          ->synonym("v")
          ->comma()
          ->synonym("y")
          ->closeBracket()
          ->build()
  ), QPSParserSyntaxError);
}

TEST_CASE("Test PQL Assign Pattern invalid arg0 synonym types") {
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
        testAssignPatternParsing(
            make_unique<PQLTestTokenSequenceBuilder>()
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

TEST_CASE("Test PQL Assign Pattern invalid arg1 synonym types") {
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
        testAssignPatternParsing(
            make_unique<PQLTestTokenSequenceBuilder>()
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
