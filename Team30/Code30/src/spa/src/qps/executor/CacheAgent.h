#pragma once

#include <utility>

#include "qps/common/CacheTable.h"

using std::unordered_set;

class CacheAgent {
  CacheTable nextTCache;
  CacheTable affectsCache;
  CacheTable affectsTCache;

 public:
  void addToNextTCache(QueryResult<StmtValue, StmtValue>* entries);
  void addToAffectsCache(QueryResult<StmtValue, StmtValue>* entries);
  void addToAffectsTCache(QueryResult<StmtValue, StmtValue>* entries);
};