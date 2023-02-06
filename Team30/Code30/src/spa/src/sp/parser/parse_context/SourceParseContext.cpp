#include "SourceParseContext.h"

SourceToken *SourceParseContext::expectVarchar(
    SourceParseState *parserState) {
  SourceToken* currentToken = parserState->getCurrToken();

  if (currentToken == nullptr) {
    throw SPError("End of token stream");
  }

  if (currentToken->isVarchar()) {
    parserState->advanceToken();
    return currentToken;
  }
  throw SPError("Unexpected Token");
}
