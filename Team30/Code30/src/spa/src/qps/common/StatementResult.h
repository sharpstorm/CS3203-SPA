#pragma once

#include <unordered_set>
#include "common/Types.h"

using std::unordered_set;

class StatementResult {
 public:
  unordered_set<int> lines;
  pair_set<int, int> linePairs;
  bool isLeftArg;
  bool isEmpty();
  bool operator == (StatementResult sr) const;
};
