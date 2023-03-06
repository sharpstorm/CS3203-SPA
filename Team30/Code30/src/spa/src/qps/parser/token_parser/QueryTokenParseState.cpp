#include "QueryTokenParseState.h"

QueryTokenParseState::QueryTokenParseState(vector<PQLToken> *tokens):
    tokenStream(tokens),
    QueryExpectationAgent(&tokenStream),
    currentStage(TOKEN_PARSE_STAGE_INIT) {
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

QueryBuilder* QueryTokenParseState::getQueryBuilder() {
  return &queryBuilder;
}

void QueryTokenParseState::advanceStage(TokenParsingStage newStage) {
  unordered_set<TokenParsingStage> allowed =
      parsingAllowedTransitions[currentStage];
  if (allowed.find(newStage) == allowed.end()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_TOKEN_SEQUENCE);
  }

  currentStage = newStage;
}
