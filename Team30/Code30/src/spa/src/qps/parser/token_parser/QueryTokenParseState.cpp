#include "QueryTokenParseState.h"

QueryTokenParseState::QueryTokenParseState(PQLTokenVector *tokens):
    tokenStream(tokens) {
}

bool QueryTokenParseState::isTokenStreamEnd() {
  return tokenStream.isTokenStreamEnd();
}

void QueryTokenParseState::advanceToken() {
  tokenStream.advanceToken();
}

PQLTokenType QueryTokenParseState::getCurrentTokenType() {
  PQLToken* curToken = tokenStream.getCurrentToken();
  if (curToken == nullptr) {
    return PQL_TOKEN_NULL;
  }

  return curToken->getType();
}

bool QueryTokenParseState::isCurrentTokenType(PQLTokenType type) {
  return getCurrentTokenType() == type;
}

bool QueryTokenParseState::isCurrentTokenCategory(PQLTokenCategory category) {
  PQLToken* curToken = tokenStream.getCurrentToken();
  if (curToken == nullptr) {
    return false;
  }

  return curToken->isCategory(category);
}

PQLSynonymName QueryTokenParseState::expectSynName() {
  PQLToken* currentToken = tokenStream.getCurrentToken();
  assertNotNull(currentToken);

  if (!currentToken->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  tokenStream.advanceToken();
  return currentToken->getData();
}

void QueryTokenParseState::assertNotNull(PQLToken* token) {
  if (token == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }
}
