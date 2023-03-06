#include "QueryTokenParseState.h"

QueryTokenParseState::QueryTokenParseState(vector<PQLToken> *tokens):
    tokenStream(tokens),
    QueryExpectationAgent(&tokenStream) {
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
