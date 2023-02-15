#include "QueryTokenParseState.h"
#include "../../errors/QPSParserError.h"

QueryTokenParseState::QueryTokenParseState(vector<PQLToken> *tokens):
    tokenStream(tokens), currentStage(TOKEN_PARSE_STAGE_INIT) {
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
    throw QPSParserError("Unexpected sequence of clauses");
  }

  currentStage = newStage;
}
