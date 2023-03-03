#pragma once

#include <vector>
#include <memory>
#include "../common/PQLQuery.h"
#include "../common/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "QueryLauncher.h"
#include "SynonymResultTable.h"
#include "qps/executor/planner/QueryPlan.h"

using std::vector;

class QueryOrchestrator {
 public:
  explicit QueryOrchestrator(QueryLauncher launcher);
//  PQLQueryResult* execute(QueryPlan* plan);
  SynonymResultTable* execute(QueryPlan* plan, PQLQuerySynonymList* targetSyns);

 private:
  QueryLauncher launcher;
  ResultCoalescer coalescer;

  PQLQueryResult* executeGroup(QueryGroupPlan* plan);
};
