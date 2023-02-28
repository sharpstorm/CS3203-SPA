#include "catch.hpp"

#include <memory>
#include <unordered_map>

#include "PQLContextTestUtils.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLCallsClauseContext.h"
#include "qps/clauses/such_that/CallsClause.h"
#include "qps/clauses/such_that/CallsTClause.h"

using std::make_unique, std::unordered_map;

void testCallsParsing(vector<PQLToken> inputs,
                      unordered_map<string, PQLSynonymType> synonyms) {
  testSuchThatParsing<PQLCallsClauseContext, CallsClause>(inputs, synonyms);
  inputs.insert(inputs.begin(), PQLToken(PQL_TOKEN_ASTRIX));
  testSuchThatParsing<PQLCallsClauseContext, CallsTClause>(inputs, synonyms);
}

void testCallsParsing(vector<PQLToken> inputs) {
  testCallsParsing(inputs,
                   unordered_map<string, PQLSynonymType>{
                       {"p", PQL_SYN_TYPE_PROCEDURE},
                       {"q", PQL_SYN_TYPE_PROCEDURE}
                   });
}

TEST_CASE("Test PQL Calls parsing 2 constants") {
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
      ->openBracket()
      ->ident("pudge")
      ->comma()
      ->ident("lion")
      ->closeBracket()
      ->build()
      );
}

TEST_CASE("Test PQL Calls 1 constant Left") {
  // Syn right
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
      ->openBracket()
      ->ident("pudge")
      ->comma()
      ->synonym("q")
      ->closeBracket()
      ->build()
      );

  // Wildcard right
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                       ->openBracket()
                       ->ident("pudge")
                       ->comma()
                       ->wildcard()
                       ->closeBracket()
                       ->build()
  );
}

TEST_CASE("Test PQL Calls 1 constant Right") {
  // Syn left
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                       ->openBracket()
                       ->synonym("p")
                       ->comma()
                       ->ident("lion")
                       ->closeBracket()
                       ->build()
  );

  // Wildcard Left
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                       ->openBracket()
                       ->wildcard()
                       ->comma()
                       ->ident("lion")
                       ->closeBracket()
                       ->build()
  );
}

TEST_CASE("Test PQL Calls both synonym") {
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                       ->openBracket()
                       ->synonym("p")
                       ->comma()
                       ->synonym("q")
                       ->closeBracket()
                       ->build()
  );
}

TEST_CASE("Test PQL Calls both wildcards") {
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                       ->openBracket()
                       ->wildcard()
                       ->comma()
                       ->wildcard()
                       ->closeBracket()
                       ->build()
  );
}

TEST_CASE("Test PQL Calls left wildcard right syn") {
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
      ->openBracket()
      ->wildcard()
      ->comma()
      ->synonym("q")
      ->closeBracket()
      ->build()
      );
}

TEST_CASE("Test PQL Calls left syn right wildcard") {
  testCallsParsing(make_unique<PQLTestTokenSequenceBuilder>()
                       ->openBracket()
                       ->synonym("p")
                       ->comma()
                       ->wildcard()
                       ->closeBracket()
                       ->build()
  );
}

