#pragma once

#include <vector>
#include<utility>

#include "common/Types.h"

using std::vector;

class CacheTable {
  vector<vector<StmtValue>> fullTable;
  vector<vector<StmtValue>> partialTable;

 public:
  void addToFullTableAt(int idx, int value);
  void addToPartialTableAt(int idx, int value);
};