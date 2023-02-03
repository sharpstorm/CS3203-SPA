#include "AbstractPQLContext.h"

PQLToken *AbstractPQLContext::expectVarchar(TokenParseState *parserState) {
  PQLToken* currentToken = parserState->getCurrentToken();
  if (currentToken == nullptr) {
    throw QPSParserError(QPS_PARSER_ERR_EOS);
  }

  if (!currentToken->isVarchar()) {
    throw QPSParserError(QPS_PARSER_ERR_UNEXPECTED);
  }

  parserState->advanceToken();
  return currentToken;
}
