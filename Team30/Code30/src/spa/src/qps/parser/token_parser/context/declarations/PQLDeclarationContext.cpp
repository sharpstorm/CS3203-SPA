#include "PQLDeclarationContext.h"

PQLDeclarationContext::PQLDeclarationContext(
    PQLSynonymType variableType):AbstractPQLContext() {
  this->variableType = variableType;
}

void PQLDeclarationContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);

  PQLToken* currentToken = parserState->expectVarchar();
  parserState->getQueryBuilder()
    ->addVariable(currentToken->getData(), variableType);

  currentToken = parserState->expect(PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  while (!currentToken->isType(PQL_TOKEN_SEMICOLON)) {
    currentToken = parserState->expectVarchar();
    parserState->getQueryBuilder()
      ->addVariable(currentToken->getData(), variableType);
    currentToken = parserState->expect(PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  }
}
