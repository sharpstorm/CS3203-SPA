#pragma once

#include <vector>
#include <memory>

#include "../common/PQLQuery.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "common/UtilityTypes.h"
#include "QueryLauncher.h"
#include "qps/common/resulttable/SynonymResultTable.h"
#include "qps/executor/planner/QueryPlan.h"
#include "QueryCache.h"

using std::vector, std::unique_ptr;

class QueryOrchestrator {
 public:
  explicit QueryOrchestrator(QueryLauncher launcher);
  SynonymResultTable* execute(QueryPlan* plan, OverrideTable* table);

 private:
  QueryLauncher launcher;

  PQLQueryResult* executeGroup(QueryGroupPlan* plan, OverrideTable* table,
                               QueryCache *cache);
};
