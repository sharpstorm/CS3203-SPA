#pragma once

#include <vector>
#include "../PQLToken.h"
#include "../builder/QueryBuilder.h"

using std::vector;

class TokenParseState {
 private:
  int currentIndex;
  int totalTokenSize;
  vector<PQLToken>* tokens;
  QueryBuilder queryBuilder;

 public:
  explicit TokenParseState(vector<PQLToken>* tokens);
  bool isEnd();
  void advanceToken();
  QueryBuilder* getQueryBuilder();
  PQLToken* getCurrentToken();
};
