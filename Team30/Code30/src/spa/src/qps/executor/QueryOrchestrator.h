#pragma once

#include <vector>
#include <memory>

#include "../common/PQLQuery.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "common/UtilityTypes.h"
#include "QueryLauncher.h"
#include "qps/common/projector_table/ProjectableTable.h"
#include "qps/executor/planner/QueryPlan.h"
#include "QueryCache.h"

using std::vector, std::unique_ptr;

class QueryOrchestrator {
 public:
  explicit QueryOrchestrator(QueryLauncher launcher);
  ProjectableTable *execute(const QueryPlan *plan,
                            const OverrideTable *table) const;

 private:
  QueryLauncher launcher;

  PQLQueryResult *executeGroup(const QueryGroupPlan *plan,
                               const OverrideTable *table,
                               QueryCache *cache) const;
};
