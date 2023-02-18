#include "PQLDeclarationContext.h"

PQLDeclarationContext::PQLDeclarationContext(PQLSynonymType synonymType):
    AbstractPQLContext(), synonymType(synonymType) {
}

void PQLDeclarationContext::parse(QueryTokenParseState *parserState) {
  parserState->advanceStage(TOKEN_PARSE_STAGE_DECLARATION);

  PQLToken* currentToken = parserState->expectSynName();
  addVariableToState(currentToken->getData(), parserState);

  currentToken = parserState->expect(PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  while (!currentToken->isType(PQL_TOKEN_SEMICOLON)) {
    currentToken = parserState->expectSynName();
    addVariableToState(currentToken->getData(), parserState);
    currentToken = parserState->expect(PQL_TOKEN_COMMA, PQL_TOKEN_SEMICOLON);
  }
}

void PQLDeclarationContext::addVariableToState(
    string name,
    QueryTokenParseState *parserState) {
  parserState->getQueryBuilder()->addSynonym(name, synonymType);
}
