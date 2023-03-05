#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {
}

SynonymResultTable *QueryExecutor::executeQuery(PQLQuery* query) {
  QueryPlanPtr plan = planner.getExecutionPlan(query);
  return orchestrator.execute(plan.get());
}
