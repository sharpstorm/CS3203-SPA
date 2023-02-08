#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using::std::shared_ptr, std::vector;

QueryExecutor::QueryExecutor(shared_ptr<PkbQueryHandler> pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {
}

PQLQueryResult *QueryExecutor::executeQuery(PQLQuery* query) {
  shared_ptr<QueryPlan> plan = planner.getExecutionPlan(query);
  PQLQueryResult* result = orchestrator.execute(query, plan);
  return result;
}
