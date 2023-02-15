#include "catch.hpp"

#include <memory>

#include "../../util/PQLTestTokenSequenceBuilder.cpp"
#include "qps/parser/token_parser/context/query/PQLSuchThatContext.h"
#include "qps/errors/QPSParserError.h"

using std::make_unique;

TEST_CASE("Test PQL Such That parsing") {
  PQLSuchThatContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->addToken(PQL_TOKEN_THAT)
      ->build();

  QueryTokenParseState state(&dummyStream);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  context.parse(&state);
}

TEST_CASE("Test PQL Such That bad symbol") {
  PQLSuchThatContext context;

  auto dummyStream = make_unique<PQLTestTokenSequenceBuilder>()
      ->addToken(PQL_TOKEN_STRING, "those")
      ->build();
  QueryTokenParseState state(&dummyStream);
  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  REQUIRE_THROWS_AS(context.parse(&state), QPSParserError);
}
