#include "PQLSelectContext.h"

void PQLSelectContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  PQLSynonymName synName = parserState->expectSynName()->getData();
  PQLQuerySynonym* synonym = parserState->getQueryBuilder()
      ->accessSynonym(synName);

  if (synonym == nullptr) {
    return;
  }

  parserState->getQueryBuilder()->setResultSynonym(
      synonym->getType(),
      synonym->getName());
}
