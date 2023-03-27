#pragma once

#include <vector>
#include<utility>

#include "common/Types.h"

using std::vector;

class CacheTable {
  vector<vector<StmtValue>> fullTable;
  vector<vector<StmtValue>> partialTable;

 public:
  void addToFullTable(const vector<StmtValue>& tableEntry);
  void addToPartialTable(const vector<StmtValue>& tableEntry);
};