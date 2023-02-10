#pragma once

#include <vector>
#include <memory>
#include "../common/PQLQuery.h"
#include "../common/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "QueryLauncher.h"
#include "QueryPlan.h"

using std::vector;

class QueryOrchestrator {
    QueryLauncher launcher;
    ResultCoalescer coalescer;
 public:
  explicit QueryOrchestrator(QueryLauncher launcher);
  PQLQueryResult* execute(PQLQuery* query, QueryPlan* plan);

 private:
  QueryLauncher launcher;
  ResultCoalescer coalescer;
  bool shouldExecuteSelect(QueryPlan* plan, PQLQueryResult* state);
};
