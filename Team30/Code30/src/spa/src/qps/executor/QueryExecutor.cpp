#include <vector>
#include <memory>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using::std::shared_ptr, std::vector;

QueryExecutor::QueryExecutor(QueryOrchestrator orchestrator) :
        orchestrator(orchestrator) {
}

PQLQueryResult *QueryExecutor::executeQuery(PQLQuery* query) {
  vector<shared_ptr<IEvaluatable>> evaluatables =
      planner.getExecuteOrder(query);
  PQLQueryResult* result = orchestrator.execute(query, evaluatables);
  return result;
}
