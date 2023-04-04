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
  QueryExecutorAgent(const PkbQueryHandler* pkb,
                     const OverrideTable* table,
                     QueryCache *cache);
  StmtRef transformArg(const PQLSynonymName name, const StmtRef &ref) const;
  EntityRef transformArg(const PQLSynonymName name, const EntityRef &ref) const;
  bool isValid(const StmtRef &ref) const;
  bool isValid(const EntityRef &ref) const;
  CacheTable* getAffectsCache() const;

  const PkbQueryHandler* operator->() const;

 private:
  const PkbQueryHandler* pkbQueryHandler;
  const OverrideTable* overrideTable;
  QueryCache *cache;
};
