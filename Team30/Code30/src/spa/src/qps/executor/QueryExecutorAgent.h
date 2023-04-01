#pragma once

#include <memory>

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/common/CacheTable.h"
#include "QueryCache.h"

typedef QueryResult<StmtValue, StmtValue> StmtStmtQueryResult;
typedef unique_ptr<StmtStmtQueryResult> StmtStmtQueryResultPtr;

using std::unique_ptr;

class QueryExecutorAgent {
 public:
  QueryExecutorAgent(PkbQueryHandler* pkb, OverrideTable* table,
                     QueryCache *cache);
  StmtRef transformArg(const PQLSynonymName name, const StmtRef &ref) const;
  EntityRef transformArg(const PQLSynonymName name, const EntityRef &ref) const;
  bool isValid(const StmtRef &ref) const;
  bool isValid(const EntityRef &ref) const;
  bool entryExistsNextT(const StmtRef& left, const StmtRef& right);
  bool entryExistsAffects(const StmtRef& left, const StmtRef& right);
  CacheTable* getNextTCache() const;
  CacheTable* getAffectsCache() const;

  PkbQueryHandler* operator->() const;

 private:
  PkbQueryHandler* pkbQueryHandler;
  OverrideTable* overrideTable;
  QueryCache *cache;

  static bool isValidRef(StmtRef stmtRef, PkbQueryHandler* handler);
  static bool isValidRef(EntityRef entRef, PkbQueryHandler* handler);

  void addToNextTCache(StmtStmtQueryResult* result);
  void addToAffectsCache(StmtStmtQueryResult* result);

  StmtStmtQueryResultPtr queryNextTCache(const StmtRef& left,
                                         const StmtRef& right);
  StmtStmtQueryResultPtr queryAffectsCache(const StmtRef& left,
                                           const StmtRef& right);
  static StmtStmtQueryResultPtr toQueryResult(const StmtRef& left,
                                       const StmtRef& right,
                                       CacheRow* row);
};
