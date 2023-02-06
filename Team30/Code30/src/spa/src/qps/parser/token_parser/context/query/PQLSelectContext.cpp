#include "PQLSelectContext.h"

void PQLSelectContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  PQLToken* currentToken = expectVarchar(parserState);
  PQLSynonymType* type = parserState->getQueryBuilder()
      ->getVariableType(currentToken->tokenData);
  if (type == nullptr) {
    throw QPSParserError("Select clause references unknown synonym");
  }

  parserState->getQueryBuilder()->setResultType(*type);
  parserState->getQueryBuilder()->setResultVariable(currentToken->tokenData);
}
