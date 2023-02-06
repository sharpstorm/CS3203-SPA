#include "catch.hpp"

#include "qps/parser/token_parser/context/declarations/PQLStmtContext.h"
#include "qps/errors/QPSParserError.h"

TEST_CASE("Test PQL Declaration parsing") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STRING, "s"},
      PQLToken{PQL_TOKEN_SEMICOLON},
  };
  QueryTokenParseState state(&dummyStream);
  context.parse(&state);
  REQUIRE(state.getQueryBuilder()->hasVariable("s"));
}

TEST_CASE("Test PQL Declaration Duplicated Name") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT, "stmt"},
      PQLToken{PQL_TOKEN_SEMICOLON},
  };
  QueryTokenParseState state(&dummyStream);
  context.parse(&state);
  REQUIRE(state.getQueryBuilder()->hasVariable("stmt"));
}

TEST_CASE("Test PQL Declaration Bad Symbol") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_COMMA},
  };
  QueryTokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}

TEST_CASE("Test PQL Declaration No semicolon") {
  PQLStmtContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STMT, "stmt"},
  };
  QueryTokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
