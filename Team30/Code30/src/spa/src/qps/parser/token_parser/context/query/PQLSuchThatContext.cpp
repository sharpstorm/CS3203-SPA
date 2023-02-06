#include "PQLSuchThatContext.h"

void PQLSuchThatContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  expect(parserState, PQL_TOKEN_THAT);
}
