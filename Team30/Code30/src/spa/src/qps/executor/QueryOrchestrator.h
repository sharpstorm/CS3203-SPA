#pragma once

#include <vector>
#include <memory>

#include "../common/PQLQuery.h"
#include "../common/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "common/UtilityTypes.h"
#include "QueryLauncher.h"
#include "SynonymResultTable.h"
#include "qps/executor/planner/QueryPlan.h"

using std::vector, std::unique_ptr;

class QueryOrchestrator {
 public:
  explicit QueryOrchestrator(QueryLauncher launcher);
  SynonymResultTable* execute(QueryPlan* plan);

 private:
  QueryLauncher launcher;
  ResultCoalescer coalescer;

  PQLQueryResult* executeGroup(QueryGroupPlan* plan);
};
