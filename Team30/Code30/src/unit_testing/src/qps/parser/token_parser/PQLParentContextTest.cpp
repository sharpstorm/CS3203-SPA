#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLParentClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/ParentClause.h"
#include "qps/parser/builder/QueryBuilderError.h"

using std::make_unique, std::unordered_map;

void testParentParsing(vector<PQLToken> inputs,
                       unordered_map<string, PQLSynonymType> synonyms) {
  PQLParentClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);

  for (auto it : synonyms) {
    state.getQueryBuilder()->addVariable(it.first, it.second);
  }

  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<ParentClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

void testParentParsing(vector<PQLToken> inputs) {
  testParentParsing(inputs, unordered_map<string, PQLSynonymType>{
      { "s", PQL_VAR_TYPE_STMT }
  });
}

TEST_CASE("Test PQL Parent parsing 2 Constants") {
  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->integer(2)
                        ->comma()
                        ->integer(1)
                        ->closeBracket()
                        ->build());
}

TEST_CASE("Test PQL Parent 1 Constant Left") {
  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->synonym("s")
                        ->comma()
                        ->integer(1)
                        ->closeBracket()
                        ->build());

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->wildcard()
                        ->comma()
                        ->integer(1)
                        ->closeBracket()
                        ->build());
}

TEST_CASE("Test PQL Parent 1 Constant Right") {
  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->integer(1)
                        ->comma()
                        ->synonym("s")
                        ->closeBracket()
                        ->build());

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->integer(1)
                        ->comma()
                        ->wildcard()
                        ->closeBracket()
                        ->build());
}

TEST_CASE("Test PQL Parent 0 Constant") {
  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->synonym("s")
                        ->comma()
                        ->synonym("s")
                        ->closeBracket()
                        ->build());

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->wildcard()
                        ->comma()
                        ->synonym("s")
                        ->closeBracket()
                        ->build());

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->synonym("s")
                        ->comma()
                        ->wildcard()
                        ->closeBracket()
                        ->build());

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->wildcard()
                        ->comma()
                        ->wildcard()
                        ->closeBracket()
                        ->build());
}

TEST_CASE("Test PQL Parent unknown ref") {
  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->synonym("w")
              ->comma()
              ->integer(2)
              ->closeBracket()
              ->build()), QPSParserError
  );
}

TEST_CASE("Test PQL Parent Entity ref not allowed") {
  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->ident("w")
              ->comma()
              ->integer(2)
              ->closeBracket()
              ->build()), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->integer(2)
              ->comma()
              ->ident("w")
              ->closeBracket()
              ->build()), QPSParserError
  );
}

TEST_CASE("Test PQL Parent bad syntax") {
  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->integer(2)
              ->synonym("w")
              ->closeBracket()
              ->build()), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->integer(2)
              ->comma()
              ->synonym("w")
              ->build()), QPSParserError
  );
}

TEST_CASE("Test PQL Parent valid synonym types") {
  auto validTypes = vector<PQLSynonymType>{
      PQL_VAR_TYPE_STMT,
      PQL_VAR_TYPE_ASSIGN,
      PQL_VAR_TYPE_PRINT,
      PQL_VAR_TYPE_IF,
      PQL_VAR_TYPE_CALL,
      PQL_VAR_TYPE_WHILE,
      PQL_VAR_TYPE_READ
  };

  for (PQLSynonymType type : validTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"s1", type},
        {"s2", type}
    };
    testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("s1")
                          ->comma()
                          ->integer(2)
                          ->closeBracket()
                          ->build(),
                      synonymMap
    );

    testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(2)
                          ->comma()
                          ->synonym("s1")
                          ->closeBracket()
                          ->build(),
                      synonymMap
    );

    testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("s1")
                          ->comma()
                          ->synonym("s2")
                          ->closeBracket()
                          ->build(),
                      synonymMap
    );
  }
}

TEST_CASE("Test PQL Parent invalid synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_VAR_TYPE_VARIABLE,
      PQL_VAR_TYPE_CONSTANT,
      PQL_VAR_TYPE_PROCEDURE
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"s1", type},
        {"s2", type}
    };

    REQUIRE_THROWS_AS(
        testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("s1")
                              ->comma()
                              ->integer(2)
                              ->closeBracket()
                              ->build(),
                          synonymMap
        ), QueryBuilderError
    );

    REQUIRE_THROWS_AS(
        testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->integer(2)
                              ->comma()
                              ->synonym("s1")
                              ->closeBracket()
                              ->build(),
                          synonymMap
        ), QueryBuilderError
    );

    REQUIRE_THROWS_AS(
        testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("s1")
                              ->comma()
                              ->synonym("s2")
                              ->closeBracket()
                              ->build(),
                          synonymMap
        ), QueryBuilderError
    );
  }
}
