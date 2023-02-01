#include <vector>
#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

QueryResult *QueryExecutor::executeQuery(PQLQuery* query) {
    vector<IEvaluatable>* evaluatables = planner.getExecuteOrder(query);
    QueryResult* result = orchestrator.execute(query, evaluatables);
    return result;
}
