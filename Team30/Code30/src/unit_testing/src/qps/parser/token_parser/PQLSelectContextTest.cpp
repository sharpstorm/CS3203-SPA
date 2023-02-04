#include "catch.hpp"

#include "qps/parser/token_parser/context/query/PQLSelectContext.h"
#include "qps/errors/QPSParserError.h"

TEST_CASE("Test PQL Select parsing") {
  PQLSelectContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STRING, "s"},
  };
  TokenParseState state(&dummyStream);
  state.getQueryBuilder()->addVariable("s", PQL_VAR_TYPE_STMT);
  context.parse(&state);

  auto resultVar = state.getQueryBuilder()->build()->getResultVariable();
  REQUIRE(resultVar.name == "s");
  REQUIRE(resultVar.type == PQL_VAR_TYPE_STMT);
}

TEST_CASE("Test PQL Select unknown synonym") {
  PQLSelectContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STRING, "s"},
  };
  TokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}

TEST_CASE("Test PQL Select bad symbol") {
  PQLSelectContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_SEMICOLON},
  };
  TokenParseState state(&dummyStream);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
