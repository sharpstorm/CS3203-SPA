#include "catch.hpp"

#include "qps/parser/token_parser/context/query/PQLSuchThatContext.h"
#include "qps/errors/QPSParserError.h"

TEST_CASE("Test PQL Such That parsing") {
  PQLSuchThatContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_THAT, "that"},
  };
  QueryTokenParseState state(&dummyStream);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  context.parse(&state);
}

TEST_CASE("Test PQL Such That bad symbol") {
  PQLSuchThatContext context;

  auto dummyStream = vector<PQLToken>{
      PQLToken{PQL_TOKEN_STRING, "those"},
  };
  QueryTokenParseState state(&dummyStream);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
