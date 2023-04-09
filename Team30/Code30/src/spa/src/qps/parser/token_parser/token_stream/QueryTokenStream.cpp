#include "QueryTokenStream.h"

QueryTokenStream::QueryTokenStream(PQLTokenList *tokens) {
  this->tokens = tokens;
  currentIndex = 0;
  totalTokenSize = tokens->size();
}

const bool QueryTokenStream::isTokenStreamEnd() const {
  return currentIndex >= totalTokenSize;
}

void QueryTokenStream::advanceToken() {
  if (!isTokenStreamEnd()) {
    currentIndex++;
  }
}

const PQLToken *QueryTokenStream::getCurrentToken() const {
  if (isTokenStreamEnd()) {
    return nullptr;
  }

  return &tokens->at(currentIndex);
}
