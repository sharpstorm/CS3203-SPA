#include "PQLSuchThatContext.h"

void PQLSuchThatContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_CONDITION_MARKER);
  parserState->expect(PQL_TOKEN_THAT);
}
