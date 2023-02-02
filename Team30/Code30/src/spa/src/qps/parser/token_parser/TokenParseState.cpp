#include "TokenParseState.h"

TokenParseState::TokenParseState(vector<PQLToken> *tokens) {
  this->tokens = tokens;
  currentIndex = 0;
  totalTokenSize = tokens->size();
}

bool TokenParseState::isEnd() {
  return currentIndex >= totalTokenSize;
}

void TokenParseState::advanceToken() {
  if (!isEnd()) {
    currentIndex++;
  }
}

PQLToken* TokenParseState::getCurrentToken() {
  if (isEnd()) {
    return nullptr;
  }

  return &tokens->at(currentIndex);
}

QueryBuilder* TokenParseState::getQueryBuilder() {
  return &queryBuilder;
}
