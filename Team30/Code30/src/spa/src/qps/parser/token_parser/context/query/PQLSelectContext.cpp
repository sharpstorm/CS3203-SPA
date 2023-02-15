#include "PQLSelectContext.h"

void PQLSelectContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  PQLToken* currentToken = parserState->expectVarchar();
  PQLQuerySynonym* synonym = parserState->getQueryBuilder()
      ->getVariable(currentToken->getData());
  if (synonym == nullptr) {
    throw QPSParserError("Select clause references unknown synonym");
  }

  parserState->getQueryBuilder()->setResultVariable(
      synonym->getType(),
      synonym->getName()
  );
}
