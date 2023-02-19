#include "SourceParseContext.h"

SourceToken *SourceParseContext::expectVarchar(
    SourceParseState *parserState) {
  SourceToken* currentToken = parserState->getCurrToken();

  if (currentToken == nullptr) {
    throw SPError(SPERR_END_OF_STREAM);
  }

  if (currentToken->isVarchar()) {
    parserState->advanceToken();
    return currentToken;
  }
  throw SPError(SPERR_UNEXPECTED_TOKEN);
}
