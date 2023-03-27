#pragma once

#include <utility>

#include "qps/common/CacheTable.h"

using std::unordered_set;

typedef QueryResult<StmtValue, StmtValue> StmtStmtQueryResult;

class CacheAgent {
  CacheTable nextTCache;
  CacheTable affectsCache;
  CacheTable affectsTCache;

 public:
  void addToNextTCache(StmtStmtQueryResult* entries);
  void addToAffectsCache(StmtStmtQueryResult* entries);
  void addToAffectsTCache(StmtStmtQueryResult* entries);
  bool inNextTCache();
  bool inAffectsCache();
  bool inAffectsTCache();
};
