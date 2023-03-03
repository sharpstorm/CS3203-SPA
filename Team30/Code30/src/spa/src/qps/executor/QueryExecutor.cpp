#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {
}

PQLQueryResult *QueryExecutor::executeQuery(PQLQuery* query) {
  QueryPlanPtr plan = planner.getExecutionPlan(query);
  PQLQueryResult* result = orchestrator.execute(plan.get());
  return result;
}
