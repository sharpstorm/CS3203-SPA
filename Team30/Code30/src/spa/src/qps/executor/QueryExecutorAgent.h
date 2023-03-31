#pragma once

#include "CacheAgent.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/common/CacheTable.h"

class QueryExecutorAgent {
 public:
  QueryExecutorAgent(PkbQueryHandler* pkb, OverrideTable* table);
  StmtRef transformArg(const PQLSynonymName name, const StmtRef &ref) const;
  EntityRef transformArg(const PQLSynonymName name, const EntityRef &ref) const;
  bool isValid(const StmtRef &ref) const;
  bool isValid(const EntityRef &ref) const;

  PkbQueryHandler* operator->() const;

 private:
  PkbQueryHandler* pkbQueryHandler;
  OverrideTable* overrideTable;
  CacheAgent cacheAgent;

  static bool isValidRef(StmtRef stmtRef, PkbQueryHandler* handler);
  static bool isValidRef(EntityRef entRef, PkbQueryHandler* handler);

  void addToNextTCache(StmtStmtQueryResult* result);
  void addToAffectsCache(StmtStmtQueryResult* result);

  StmtStmtQueryResultPtr queryNextTCache(const StmtRef& left,
                                         const StmtRef& right);
  StmtStmtQueryResultPtr queryAffectsCache(const StmtRef& left,
                                           const StmtRef& right);
  bool entryExistsNextT(StmtRef left, StmtRef right);
  bool entryExistsAffects(StmtRef left, StmtRef right);
};
