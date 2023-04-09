#include "QueryTokenParseState.h"

QueryTokenParseState::QueryTokenParseState(PQLTokenList *tokens) :
    tokenStream(tokens) {
}

bool QueryTokenParseState::isTokenStreamEnd() const {
  return tokenStream.isTokenStreamEnd();
}

void QueryTokenParseState::advanceToken() {
  tokenStream.advanceToken();
}

PQLTokenType QueryTokenParseState::getCurrentTokenType() const {
  const PQLToken *curToken = tokenStream.getCurrentToken();
  if (curToken == nullptr) {
    return PQL_TOKEN_NULL;
  }

  return curToken->getType();
}

bool QueryTokenParseState::isCurrentTokenType(PQLTokenType type) const {
  return getCurrentTokenType() == type;
}

bool QueryTokenParseState::isCurrentTokenCategory(PQLTokenCategory category)
const {
  const PQLToken *curToken = tokenStream.getCurrentToken();
  if (curToken == nullptr) {
    return false;
  }

  return curToken->isCategory(category);
}

PQLSynonymName QueryTokenParseState::expectSynName() {
  const PQLToken *currentToken = tokenStream.getCurrentToken();
  assertNotNull(currentToken);

  if (!currentToken->isSynName()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  tokenStream.advanceToken();
  return currentToken->getData();
}

void QueryTokenParseState::assertNotNull(const PQLToken *token) {
  if (token == nullptr) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_EOS);
  }
}
