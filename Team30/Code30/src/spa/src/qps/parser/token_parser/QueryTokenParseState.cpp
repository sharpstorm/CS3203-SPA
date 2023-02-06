#include "QueryTokenParseState.h"
#include "../../errors/QPSParserError.h"

QueryTokenParseState::QueryTokenParseState(vector<PQLToken> *tokens) {
  this->tokens = tokens;
  currentIndex = 0;
  totalTokenSize = tokens->size();
  currentStage = TOKEN_PARSE_STAGE_INIT;
}

bool QueryTokenParseState::isTokenStreamEnd() {
  return currentIndex >= totalTokenSize;
}

void QueryTokenParseState::advanceToken() {
  if (!isTokenStreamEnd()) {
    currentIndex++;
  }
}

PQLToken* QueryTokenParseState::getCurrentToken() {
  if (isTokenStreamEnd()) {
    return nullptr;
  }

  return &tokens->at(currentIndex);
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
