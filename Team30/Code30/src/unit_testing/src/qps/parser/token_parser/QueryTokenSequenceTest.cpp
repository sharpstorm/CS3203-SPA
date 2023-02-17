#include "catch.hpp"

#include <vector>
#include "qps/parser/token_parser/QueryTokenParseState.h"
#include "qps/errors/QPSParserSyntaxError.h"

using std::vector, std::make_unique;

TEST_CASE("Test QueryTokenParseState Stage Valid Flows") {
  auto dummyStream = vector<PQLToken>{
    PQLToken(PQL_TOKEN_SELECT)
  };

  unique_ptr<QueryTokenParseState> state = make_unique<QueryTokenParseState>(&dummyStream);
  state->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);
  state->advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state->advanceStage(TOKEN_PARSE_STAGE_PARSE_END);

  state = make_unique<QueryTokenParseState>(&dummyStream);
  state->advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state->advanceStage(TOKEN_PARSE_STAGE_PARSE_END);

  state = make_unique<QueryTokenParseState>(&dummyStream);
  state->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);
  state->advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state->advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state->advanceStage(TOKEN_PARSE_STAGE_CONDITION);
  state->advanceStage(TOKEN_PARSE_STAGE_PARSE_END);

  state = make_unique<QueryTokenParseState>(&dummyStream);
  state->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);
  state->advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  state->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  state->advanceStage(TOKEN_PARSE_STAGE_PARSE_END);

  state = make_unique<QueryTokenParseState>(&dummyStream);
  state->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);
  state->advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state->advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state->advanceStage(TOKEN_PARSE_STAGE_CONDITION);
  state->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  state->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  state->advanceStage(TOKEN_PARSE_STAGE_PARSE_END);

  state = make_unique<QueryTokenParseState>(&dummyStream);
  state->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);
  state->advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  state->advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  state->advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  state->advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  state->advanceStage(TOKEN_PARSE_STAGE_CONDITION);
  state->advanceStage(TOKEN_PARSE_STAGE_PARSE_END);
}

TEST_CASE("Test QueryTokenParseState Stage Invalid Flows") {
  auto dummyStream = vector<PQLToken>{
      PQLToken(PQL_TOKEN_SELECT)
  };
  QueryTokenParseState state(&dummyStream);

  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END), QPSParserSyntaxError);

  state.advanceStage(TOKEN_PARSE_STAGE_DECLARATION);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END), QPSParserSyntaxError);

  state.advanceStage(TOKEN_PARSE_STAGE_COMMAND);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_DECLARATION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN), QPSParserSyntaxError);

  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_DECLARATION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_COMMAND), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END), QPSParserSyntaxError);

  state.advanceStage(TOKEN_PARSE_STAGE_CONDITION);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_DECLARATION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_COMMAND), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PATTERN), QPSParserSyntaxError);

  state.advanceStage(TOKEN_PARSE_STAGE_PATTERN_MARKER);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_DECLARATION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_COMMAND), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END), QPSParserSyntaxError);

  state.advanceStage(TOKEN_PARSE_STAGE_PATTERN);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_DECLARATION), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_COMMAND), QPSParserSyntaxError);
  REQUIRE_THROWS_AS(state.advanceStage(TOKEN_PARSE_STAGE_CONDITION), QPSParserSyntaxError);
}
