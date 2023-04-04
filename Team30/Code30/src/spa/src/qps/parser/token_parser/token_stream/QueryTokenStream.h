#pragma once

#include "qps/parser/PQLToken.h"

class QueryTokenStream {
 private:
  int currentIndex;
  int totalTokenSize;
  PQLTokenVector *tokens;

 public:
  explicit QueryTokenStream(PQLTokenVector *tokens);
  const bool isTokenStreamEnd() const;
  void advanceToken();
  const PQLToken *getCurrentToken() const;
};
