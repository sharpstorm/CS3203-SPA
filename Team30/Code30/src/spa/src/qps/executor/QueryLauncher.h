#pragma once

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"
#include "QueryCache.h"

class QueryLauncher {
  PkbQueryHandler* pkbQueryHandler;
 public:
  explicit QueryLauncher(PkbQueryHandler* pkbQH);
  PQLQueryResult* execute(IEvaluatable* evaluatable, OverrideTable * table,
                          QueryCache *cache);
};
