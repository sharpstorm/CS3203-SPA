#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "PQLContextTestUtils.h"
#include "qps/errors/QPSParserSyntaxError.h"
#include "qps/errors/QPSParserSemanticError.h"
#include "qps/parser/token_parser/parsers/conditional_parser/such_that_parser/contexts/PQLSuchThatClauseTypes.h"

using std::make_unique, std::unordered_map;

void testFollowsParsing(vector<PQLToken> inputs,
                        unordered_map<string, PQLSynonymType> synonyms) {
  testSuchThatParsing<PQLFollowsClauseContext, FollowsClause>(inputs,
                                                              synonyms);
  inputs.insert(inputs.begin(), PQLToken(PQL_TOKEN_ASTRIX));
  testSuchThatParsing<PQLFollowsClauseContext, FollowsTClause>(inputs,
                                                              synonyms);
}

void testFollowsParsing(vector<PQLToken> inputs) {
  testFollowsParsing(
      inputs,
      unordered_map<string, PQLSynonymType>{
          {"s", PQL_SYN_TYPE_STMT }
      }
  );
}

TEST_CASE("Test PQL Follows parsing 2 Constants") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(1)
                         ->comma()
                         ->integer(2)
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows 1 Constant Left") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->synonym("s")
                         ->comma()
                         ->integer(2)
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->wildcard()
                         ->comma()
                         ->integer(2)
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows 1 Constant Right") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(2)
                         ->comma()
                         ->synonym("s")
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(2)
                         ->comma()
                         ->wildcard()
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows 0 Constant") {
  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->synonym("s")
                         ->comma()
                         ->synonym("s")
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->synonym("s")
                         ->comma()
                         ->wildcard()
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->wildcard()
                         ->comma()
                         ->synonym("s")
                         ->closeBracket()
                         ->build()
  );

  testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->wildcard()
                         ->comma()
                         ->wildcard()
                         ->closeBracket()
                         ->build()
  );
}

TEST_CASE("Test PQL Follows unknown ref") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->synonym("w")
                             ->comma()
                             ->integer(2)
                             ->closeBracket()
                             ->build()
      ), QPSParserSemanticError
  );
}

TEST_CASE("Test PQL Follows Entity ref not allowed") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->ident("w")
                             ->comma()
                             ->integer(2)
                             ->closeBracket()
                             ->build()
      ), QPSParserSyntaxError
  );

  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->comma()
                             ->ident("w")
                             ->closeBracket()
                             ->build()
      ), QPSParserSyntaxError
  );
}

TEST_CASE("Test PQL Follows Zero Stmt not Allowed") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(0)
                             ->comma()
                             ->integer(2)
                             ->closeBracket()
                             ->build()
      ), QPSParserSemanticError
  );

  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->comma()
                             ->integer(0)
                             ->closeBracket()
                             ->build()
      ), QPSParserSemanticError
  );
}

TEST_CASE("Test PQL Follows bad syntax") {
  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->synonym("w")
                             ->closeBracket()
                             ->build()
      ), QPSParserSyntaxError
  );

  REQUIRE_THROWS_AS(
      testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                             ->openBracket()
                             ->integer(2)
                             ->comma()
                             ->synonym("w")
                             ->build()
      ), QPSParserSyntaxError
  );
}

TEST_CASE("Test PQL Follows valid synonym types") {
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
    testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                           ->openBracket()
                           ->synonym("s1")
                           ->comma()
                           ->integer(2)
                           ->closeBracket()
                           ->build(),
                       synonymMap
    );

    testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                           ->openBracket()
                           ->integer(2)
                           ->comma()
                           ->synonym("s1")
                           ->closeBracket()
                           ->build(),
                       synonymMap
    );

    testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
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

TEST_CASE("Test PQL Follows invalid synonym types") {
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
        testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
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
        testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
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
        testFollowsParsing(make_unique<PQLTestTokenSequenceBuilder>()
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
