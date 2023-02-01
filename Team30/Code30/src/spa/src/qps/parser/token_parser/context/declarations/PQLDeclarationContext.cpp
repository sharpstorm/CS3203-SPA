#include "PQLDeclarationContext.h"

PQLDeclarationContext::PQLDeclarationContext(PQL_VAR_TYPE variableType): AbstractPQLContext() {
  this->variableType = variableType;
}

void PQLDeclarationContext::parse(TokenParseState *parserState) {
  PQLToken* currentToken = expect(parserState, PQL_TOKEN_STRING);
  parserState->getQueryBuilder()
    ->addVariable(currentToken->tokenData, variableType);

  currentToken = expect(parserState, PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  while (!currentToken->isType(PQL_TOKEN_SEMICOLON)) {
    currentToken = expect(parserState, PQL_TOKEN_STRING);
    parserState->getQueryBuilder()
      ->addVariable(currentToken->tokenData, variableType);
    currentToken = expect(parserState, PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  }
}
