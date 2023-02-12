#include "catch.hpp"
#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLParentClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/ParentClause.h"

using std::make_unique;

void testParentParsing(vector<PQLToken> inputs) {
  PQLParentClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<ParentClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
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

