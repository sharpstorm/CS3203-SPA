#include "catch.hpp"
#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLModifiesClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/ModifiesClause.h"
#include "qps/parser/builder/QueryBuilderError.h"

using std::make_unique;

void testModifiesParsing(vector<PQLToken> inputs) {
  PQLModifiesClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  state.getQueryBuilder()->addVariable("p", PQL_VAR_TYPE_PROCEDURE);
  state.getQueryBuilder()->addVariable("v", PQL_VAR_TYPE_VARIABLE);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<ModifiesClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Modifies parsing 2 Constants") {
  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                         ->openBracket()
                         ->integer(7)
                         ->comma()
                         ->ident("s")
                         ->closeBracket()
                         ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->ident("s")
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
                          ->synonym("s")
                          ->comma()
                          ->ident("s")
                          ->closeBracket()
                          ->build()
  );

  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->ident("s")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Modifies 0 Constant") {
  testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("s")
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
                          ->synonym("s")
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
                              ->synonym("s")
                              ->comma()
                              ->synonym("w")
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("w")
                              ->comma()
                              ->synonym("v")
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Modifies Statement ref not allowed on right") {
  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("s")
                              ->comma()
                              ->integer(1)
                              ->closeBracket()
                              ->build()
      ), QPSParserError
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
      ), QueryBuilderError
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
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testModifiesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->integer(1)
                              ->comma()
                              ->ident("s")
                              ->build()
      ), QPSParserError
  );
}
