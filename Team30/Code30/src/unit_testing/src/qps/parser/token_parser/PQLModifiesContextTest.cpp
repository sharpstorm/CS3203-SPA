#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "PQLContextTestUtils.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLModifiesClauseContext.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/clauses/such_that/ModifiesClause.h"
#include "qps/errors/QPSParserSemanticError.h"

using std::make_unique, std::unordered_map;

void testModifiesParsing(vector<PQLToken> inputs,
                         unordered_map<string, PQLSynonymType> synonyms) {
  testSuchThatParsing<PQLModifiesClauseContext, ModifiesClause>(inputs,
                                                                synonyms);
}

void testModifiesParsing(vector<PQLToken> inputs) {
  testModifiesParsing(
      inputs,
      unordered_map<string, PQLSynonymType>{
          {"a", PQL_SYN_TYPE_ASSIGN},
          {"p", PQL_SYN_TYPE_PROCEDURE},
          {"v", PQL_SYN_TYPE_VARIABLE}
      }
  );
}

TEST_CASE("Test PQL Modifies parsing 2 Constants") {
  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(7)
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Modifies 1 Constant Left") {
  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->wildcard()
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Modifies 1 Constant Right") {
  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("a")
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->ident("x")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Modifies 0 Constant") {
  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("a")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("a")
                          ->comma()
                          ->wildcard()
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Modifies unknown ref") {
  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("a")
                              ->comma()
                              ->synonym("w")
                              ->closeBracket()
                              ->build()
      ), QPSParserSemanticError
  );

  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("w")
                              ->comma()
                              ->synonym("v")
                              ->closeBracket()
                              ->build()
      ), QPSParserSemanticError
  );
}

TEST_CASE("Test PQL Modifies Statement ref not allowed on right") {
  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("a")
                              ->comma()
                              ->integer(1)
                              ->closeBracket()
                              ->build()
      ), QPSParserSyntaxError
  );
}

TEST_CASE("Test PQL Modifies wildcard not allowed on left") {
  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->wildcard()
                              ->comma()
                              ->ident("s")
                              ->closeBracket()
                              ->build()
      ), QPSParserSemanticError
  );
}

TEST_CASE("Test PQL Modifies bad syntax") {
  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->integer(1)
                              ->ident("s")
                              ->closeBracket()
                              ->build()
      ), QPSParserSyntaxError
  );

  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->integer(1)
                              ->comma()
                              ->ident("s")
                              ->build()
      ), QPSParserSyntaxError
  );
}

TEST_CASE("Test PQL Modifies valid synonym types") {
  auto validTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_ASSIGN,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_PROCEDURE,
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_PRINT
  };

  for (PQLSynonymType type : validTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"s1", type},
        {"v", PQL_SYN_TYPE_VARIABLE}
    };
    testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                            ->openBracket()
                            ->synonym("s1")
                            ->comma()
                            ->synonym("v")
                            ->closeBracket()
                            ->build(),
                        synonymMap
    );
  }
}

TEST_CASE("Test PQL Modifies invalid left synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_VARIABLE,
      PQL_SYN_TYPE_CONSTANT
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"s1", type},
        {"v", PQL_SYN_TYPE_VARIABLE}
    };

    REQUIRE_THROWS_AS(
        testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                                ->openBracket()
                                ->synonym("s1")
                                ->comma()
                                ->synonym("v")
                                ->closeBracket()
                                ->build(),
                            synonymMap
        ), QPSParserSemanticError
    );
  }
}

TEST_CASE("Test PQL Modifies invalid right synonym types") {
  auto invalidTypes = vector<PQLSynonymType>{
      PQL_SYN_TYPE_CONSTANT,
      PQL_SYN_TYPE_PROCEDURE,
      PQL_SYN_TYPE_STMT,
      PQL_SYN_TYPE_READ,
      PQL_SYN_TYPE_PRINT,
      PQL_SYN_TYPE_CALL,
      PQL_SYN_TYPE_WHILE,
      PQL_SYN_TYPE_IF,
      PQL_SYN_TYPE_ASSIGN
  };

  for (PQLSynonymType type : invalidTypes) {
    auto synonymMap = unordered_map<string, PQLSynonymType>{
        {"v", type}
    };

    REQUIRE_THROWS_AS(
        testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                                ->openBracket()
                                ->integer(1)
                                ->comma()
                                ->synonym("v")
                                ->closeBracket()
                                ->build(),
                            synonymMap
        ), QPSParserSemanticError
    );
  }
}
