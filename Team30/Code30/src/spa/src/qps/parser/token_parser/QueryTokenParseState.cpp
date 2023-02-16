#include "QueryTokenParseState.h"

QueryTokenParseState::QueryTokenParseState(vector<PQLToken> *tokens):
    tokenStream(tokens),
    QueryExpectationAgent(&tokenStream),
    SemanticErrorThrower(),
    currentStage(TOKEN_PARSE_STAGE_INIT) {
}

bool QueryTokenParseState::isTokenStreamEnd() {
  return tokenStream.isTokenStreamEnd();
}

void QueryTokenParseState::advanceToken() {
  tokenStream.advanceToken();
}

PQLToken* QueryTokenParseState::getCurrentToken() {
  return tokenStream.getCurrentToken();
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
