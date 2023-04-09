#pragma once

#include "qps/parser/PQLToken.h"

class QueryTokenStream {
 private:
  int currentIndex;
  int totalTokenSize;
  PQLTokenList *tokens;

 public:
  explicit QueryTokenStream(PQLTokenList *tokens);
  const bool isTokenStreamEnd() const;
  void advanceToken();
  const PQLToken *getCurrentToken() const;
};
