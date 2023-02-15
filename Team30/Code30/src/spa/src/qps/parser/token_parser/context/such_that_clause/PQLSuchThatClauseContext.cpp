#include "PQLSuchThatClauseContext.h"

void PQLSuchThatClauseContext::parse(
    QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_CONDITION);
}
