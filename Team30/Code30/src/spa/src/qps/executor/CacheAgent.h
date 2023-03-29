#pragma once

#include <utility>

#include "qps/common/CacheTable.h"

using std::unordered_set;

typedef QueryResult<StmtValue, StmtValue> StmtStmtQueryResult;

class CacheAgent {
  CacheTable nextTCache;
  CacheTable affectsCache;
  CacheTable affectsTCache;
  StmtStmtQueryResult toQueryResult(StmtRef left, StmtRef right, CacheRow* row);

 public:
  void addToNextTCache(StmtStmtQueryResult* entries);
  void addToAffectsCache(StmtStmtQueryResult* entries);
  void addToAffectsTCache(StmtStmtQueryResult* entries);
  StmtStmtQueryResult queryNextTCache(StmtRef left, StmtRef right);
  StmtStmtQueryResult queryAffectsCache(StmtRef left, StmtRef right);
  StmtStmtQueryResult queryAffectsTCache(StmtRef left, StmtRef right);
};
