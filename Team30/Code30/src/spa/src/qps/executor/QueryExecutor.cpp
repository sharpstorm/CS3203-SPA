#include <vector>
#include <memory>
#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

QueryResult *QueryExecutor::executeQuery(PQLQuery* query) {
  vector<shared_ptr<IEvaluatable>> evaluatables =
      planner.getExecuteOrder(query);
    QueryResult* result = orchestrator.execute(query, evaluatables);
    return result;
}
