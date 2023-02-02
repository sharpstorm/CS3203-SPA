#include "PQLSelectContext.h"

void PQLSelectContext::parse(TokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  PQLToken* currentToken = expectVarchar(parserState);
  PQLSynonymType* type = parserState->getQueryBuilder()
      ->getVariable(currentToken->tokenData);

  if (type == nullptr) {
    throw new QPSParserError("Select clause references unknown synonym");
  }

  parserState->getQueryBuilder()->setResultType(*type);
  parserState->getQueryBuilder()->setResultVariable(currentToken->tokenData);
}
