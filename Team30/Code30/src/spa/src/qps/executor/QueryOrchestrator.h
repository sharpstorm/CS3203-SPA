#pragma once

#include <vector>
#include <memory>
#include "../common/PQLQuery.h"
#include "../common/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "QueryLauncher.h"
#include "qps/executor/planner/QueryPlan.h"

using std::vector;

class QueryOrchestrator {
 private:
  QueryLauncher launcher;
  ResultCoalescer coalescer;
  bool shouldExecuteSelect(QueryPlan* plan, PQLQueryResult* state);

 public:
  explicit QueryOrchestrator(QueryLauncher launcher);
  PQLQueryResult* execute(PQLQuery* query, QueryPlan* plan);
};
