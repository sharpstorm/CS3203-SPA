#include "PQLDeclarationContext.h"

PQLDeclarationContext::PQLDeclarationContext(
    PQLSynonymType variableType):AbstractPQLContext() {
  this->variableType = variableType;
}

void PQLDeclarationContext::parse(TokenParseState *parserState) {
  PQLToken* currentToken = expectVarchar(parserState);
  parserState->getQueryBuilder()
    ->addVariable(currentToken->tokenData, variableType);

  currentToken = expect(parserState, PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  while (!currentToken->isType(PQL_TOKEN_SEMICOLON)) {
    currentToken = expectVarchar(parserState);
    parserState->getQueryBuilder()
      ->addVariable(currentToken->tokenData, variableType);
    currentToken = expect(parserState, PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  }
}
