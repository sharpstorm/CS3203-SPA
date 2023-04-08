#pragma once

#include "../common/PQLQuery.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "common/UtilityTypes.h"
#include "QueryLauncher.h"
#include "qps/common/projector_table/ProjectableTable.h"
#include "qps/executor/planner/QueryPlan.h"
#include "QueryCache.h"

class QueryOrchestrator {
 public:
  explicit QueryOrchestrator(const QueryLauncher *launcher);
  ProjectableTable *execute(const QueryPlan *plan,
                            const OverrideTable *table);

 private:
  const QueryLauncher *launcher;
  ProjectableTablePtr resultTable;

  bool executeGroup(const QueryGroupPlan *targetGroup,
                               const OverrideTable *table,
                               QueryCache *cache);

  PQLQueryResult *executeClauses(const vector<IEvaluatable *> &executables,
                                 const OverrideTable *overrideTable,
                                 QueryCache *cache) const;
  void extractProjectables(const QueryGroupPlan *targetGroup,
                           PQLQueryResult *result);
};
