#include "PQLSelectContext.h"

void PQLSelectContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_COMMAND);

  PQLToken* currentToken = parserState->expectVarchar();
  PQLSynonymType* type = parserState->getQueryBuilder()
      ->getVariableType(currentToken->getData());
  if (type == nullptr) {
    throw QPSParserError("Select clause references unknown synonym");
  }

  parserState->getQueryBuilder()->setResultType(*type);
  parserState->getQueryBuilder()->setResultVariable(currentToken->getData());
}
