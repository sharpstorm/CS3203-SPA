#pragma once

#include <vector>
#include "qps/parser/PQLToken.h"

using std::vector;

class QueryTokenStream {
 private:
  int currentIndex;
  int totalTokenSize;
  vector<PQLToken>* tokens;

 public:
  explicit QueryTokenStream(vector<PQLToken>* tokens);
  bool isTokenStreamEnd();
  void advanceToken();
  PQLToken* getCurrentToken();
};
