#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "PQLContextTestUtils.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/clauses/such_that/ParentClauses.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique, std::unordered_map;

void testParentParsing(vector<PQLToken> inputs,
                       unordered_map<string, PQLSynonymType> synonyms) {
  testSuchThatParsing<PQLParentClauseContext, ParentClause>(inputs,
                                                            synonyms);
  inputs.insert(inputs.begin(), PQLToken(PQL_TOKEN_ASTRIX));
  testSuchThatParsing<PQLParentClauseContext, ParentTClause>(inputs,
                                                             synonyms);
}

void testParentParsing(vector<PQLToken> inputs) {
  testParentParsing(
      inputs,
      unordered_map<string, PQLSynonymType>{
          {"s", PQL_SYN_TYPE_STMT}
      }
  );
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
              ->build()
      ), QPSParserSemanticError
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
              ->build()), QPSParserSyntaxError
  );

  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->integer(2)
              ->comma()
              ->ident("w")
              ->closeBracket()
              ->build()), QPSParserSyntaxError
  );
}

TEST_CASE("Test PQL Parent Zero Is Not Semantic") {

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->integer(0)
                        ->comma()
                        ->integer(2)
                        ->closeBracket()
                        ->build()
  );

  testParentParsing(make_unique<PQLTestTokenSequenceBuilder>()
                        ->openBracket()
                        ->integer(2)
                        ->comma()
                        ->integer(0)
                        ->closeBracket()
                        ->build()
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
              ->build()), QPSParserSyntaxError
  );

  REQUIRE_THROWS_AS(
      testParentParsing(
          make_unique<PQLTestTokenSequenceBuilder>()
              ->openBracket()
              ->integer(2)
              ->comma()
              ->synonym("w")
              ->build()), QPSParserSyntaxError
  );
}

TEST_CASE("Test PQL Parent valid synonym types") {
  auto validTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_ASSIGN,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_READ
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
      PQL_SYN_TYPE_VARIABLE,
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_PROCEDURE
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
        ), QPSParserSemanticError
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
        ), QPSParserSemanticError
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
        ), QPSParserSemanticError
    );
  }
}
