#pragma once

#include <utility>
#include <memory>

#include "qps/common/CacheTable.h"

using std::unordered_set, std::unique_ptr;

typedef QueryResult<StmtValue, StmtValue> StmtStmtQueryResult;
typedef unique_ptr<StmtStmtQueryResult> StmtStmtQueryResultPtr;

class CacheAgent {
  CacheTable nextTCache;
  CacheTable affectsCache;
  CacheTable affectsTCache;
  static StmtStmtQueryResultPtr toQueryResult(const StmtRef& left,
                                              const StmtRef& right,
                                              CacheRow* row);

 public:
  void addToNextTCache(StmtStmtQueryResult* entries);
  void addToAffectsCache(StmtStmtQueryResult* entries);
  void addToAffectsTCache(StmtStmtQueryResult* entries);
  StmtStmtQueryResultPtr queryNextTCache(const StmtRef& left,
                                         const StmtRef& right);
  static StmtStmtQueryResultPtr queryAffectsCache(const StmtRef& left,
                                                  const StmtRef& right);
  static StmtStmtQueryResultPtr queryAffectsTCache(const StmtRef& left,
                                                   const StmtRef& right);
};
