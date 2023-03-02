#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::shared_ptr, std::vector;

QueryExecutor::QueryExecutor(shared_ptr<PkbQueryHandler> pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {
}

PQLQueryResult *QueryExecutor::executeQuery(PQLQuery* query) {
  unique_ptr<QueryPlan> plan = planner.getExecutionPlan(query);
//  PQLQueryResult* result = orchestrator.execute(plan.get());
  PQLQueryResult* result = orchestrator.execute(plan.get(),
                                                query->getResultVariables());
  return result;
}
