#include "TokenParseState.h"
#include "../../errors/QPSParserError.h"

TokenParseState::TokenParseState(vector<PQLToken> *tokens) {
  this->tokens = tokens;
  currentIndex = 0;
  totalTokenSize = tokens->size();
  currentStage = TOKEN_PARSE_STAGE_INIT;
}

bool TokenParseState::isTokenStreamEnd() {
  return currentIndex >= totalTokenSize;
}

void TokenParseState::advanceToken() {
  if (!isTokenStreamEnd()) {
    currentIndex++;
  }
}

PQLToken* TokenParseState::getCurrentToken() {
  if (isTokenStreamEnd()) {
    return nullptr;
  }

  return &tokens->at(currentIndex);
}

QueryBuilder* TokenParseState::getQueryBuilder() {
  return &queryBuilder;
}

void TokenParseState::advanceStage(TokenParsingStage newStage) {
  unordered_set<TokenParsingStage> allowed =
      parsingAllowedTransitions[currentStage];
  if (allowed.find(newStage) == allowed.end()) {
    throw QPSParserError("Unexpected sequence of clauses");
  }

  currentStage = newStage;
}
