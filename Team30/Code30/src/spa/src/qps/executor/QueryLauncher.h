#pragma once

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"
#include "QueryCache.h"

class QueryLauncher {
  const PkbQueryHandler *pkbQueryHandler;
 public:
  explicit QueryLauncher(const PkbQueryHandler *pkbQH);
  PQLQueryResult *execute(const IEvaluatable *evaluatable,
                          const OverrideTable *table,
                          QueryCache *cache) const;
};
