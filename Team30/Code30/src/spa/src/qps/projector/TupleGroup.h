#pragma once

#include <vector>
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/QueryResultItem.h"

using std::vector;

class TupleGroup {
  vector<vector<QueryResultItem>> tupleRows;
  vector<PQLSynonymName> synKeys;

 public:
  void crossProduct(unordered_map<PQLSynonymName, QueryResultItemSet> mapping);
};