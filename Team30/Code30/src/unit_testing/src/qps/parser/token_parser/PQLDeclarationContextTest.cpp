#include "catch.hpp"

#include "qps/parser/token_parser/context/declarations/PQLStmtContext.h"
#include "qps/errors/QPSParserError.h"

TEST_CASE("Test PQL Declaration parsing") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT},
      PQLToken{PQL_TOKEN_STRING, "s"},
  };
  TokenParseState state(&dummyStream);
  context.parse(&state);
  REQUIRE(state.getQueryBuilder()->hasVariable("s"));
}

TEST_CASE("Test PQL Declaration Duplicated Name") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT},
      PQLToken{PQL_TOKEN_STMT, "stmt"},
  };
  TokenParseState state(&dummyStream);
  context.parse(&state);
  REQUIRE(state.getQueryBuilder()->hasVariable("stmt"));
}

TEST_CASE("Test PQL Declaration Bad Symbol") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT},
      PQLToken{PQL_TOKEN_COMMA},
  };
  TokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
