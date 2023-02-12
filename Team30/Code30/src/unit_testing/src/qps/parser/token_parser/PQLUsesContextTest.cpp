#include "catch.hpp"
#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/such_that_clause/PQLUsesClauseContext.h"
#include "qps/errors/QPSParserError.h"
#include "qps/clauses/UsesClause.h"
#include "qps/parser/builder/QueryBuilderError.h"

using std::make_unique;

void testUsesParsing(vector<PQLToken> inputs) {
  PQLUsesClauseContext context;
  QueryTokenParseState state(&inputs);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  state.getQueryBuilder()->addVariable("p", PQL_VAR_TYPE_PROCEDURE);
  state.getQueryBuilder()->addVariable("v", PQL_VAR_TYPE_VARIABLE);
  context.parse(&state);

  auto clauses = state.getQueryBuilder()->build()->getEvaluatables();
  REQUIRE(clauses.size() == 1);

  auto fc = dynamic_cast<UsesClause*>(clauses.at(0).get());
  REQUIRE(fc != nullptr);
}

TEST_CASE("Test PQL Uses parsing 2 Constants") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(7)
                          ->comma()
                          ->ident("s")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->ident("s")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses 1 Constant Left") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->ident("main")
                          ->comma()
                          ->wildcard()
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses 1 Constant Right") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("s")
                          ->comma()
                          ->ident("s")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->ident("s")
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses 0 Constant") {
  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("s")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("p")
                          ->comma()
                          ->synonym("v")
                          ->closeBracket()
                          ->build()
  );

  testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->synonym("s")
                          ->comma()
                          ->wildcard()
                          ->closeBracket()
                          ->build()
  );
}

TEST_CASE("Test PQL Uses unknown ref") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("s")
                              ->comma()
                              ->synonym("w")
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("w")
                              ->comma()
                              ->synonym("v")
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Uses Statement ref not allowed on right") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->synonym("s")
                              ->comma()
                              ->integer(1)
                              ->closeBracket()
                              ->build()
      ), QPSParserError
  );
}

TEST_CASE("Test PQL Uses wildcard not allowed on left") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                              ->openBracket()
                              ->wildcard()
                              ->comma()
                              ->ident("s")
                              ->closeBracket()
                              ->build()
      ), QueryBuilderError
  );
}

TEST_CASE("Test PQL Uses bad syntax") {
  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->ident("s")
                          ->closeBracket()
                          ->build()
      ), QPSParserError
  );

  REQUIRE_THROWS_AS(
      testUsesParsing(make_unique<PQLTestTokenSequenceBuilder>()
                          ->openBracket()
                          ->integer(1)
                          ->comma()
                          ->ident("s")
                          ->build()
      ), QPSParserError
  );
}
