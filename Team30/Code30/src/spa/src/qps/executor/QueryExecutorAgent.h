#pragma once

#include "CacheAgent.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/common/CacheTable.h"

typedef QueryResult<StmtValue, StmtValue> StmtStmtQueryResult;
typedef unique_ptr<StmtStmtQueryResult> StmtStmtQueryResultPtr;

class QueryExecutorAgent {
 public:
  QueryExecutorAgent(PkbQueryHandler* pkb, OverrideTable* table);
  StmtRef transformArg(const PQLSynonymName name, const StmtRef &ref) const;
  EntityRef transformArg(const PQLSynonymName name, const EntityRef &ref) const;
  bool isValid(const StmtRef &ref) const;
  bool isValid(const EntityRef &ref) const;
  bool entryExistsNextT(const StmtRef& left, const StmtRef& right);
  bool entryExistsAffects(const StmtRef& left, const StmtRef& right);

  PkbQueryHandler* operator->() const;

 private:
  PkbQueryHandler* pkbQueryHandler;
  OverrideTable* overrideTable;
  CacheTable nextTCache;
  CacheTable affectsCache;

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
