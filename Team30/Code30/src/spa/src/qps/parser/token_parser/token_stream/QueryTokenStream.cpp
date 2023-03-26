#include "QueryTokenStream.h"

QueryTokenStream::QueryTokenStream(PQLTokenVector *tokens) {
  this->tokens = tokens;
  currentIndex = 0;
  totalTokenSize = tokens->size();
}

const bool QueryTokenStream::isTokenStreamEnd() {
  return currentIndex >= totalTokenSize;
}

void QueryTokenStream::advanceToken() {
  if (!isTokenStreamEnd()) {
    currentIndex++;
  }
}

PQLToken* QueryTokenStream::getCurrentToken() {
  if (isTokenStreamEnd()) {
    return nullptr;
  }

  return &tokens->at(currentIndex);
}
