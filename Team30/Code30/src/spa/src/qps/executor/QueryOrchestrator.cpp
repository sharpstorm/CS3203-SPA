#include "QueryOrchestrator.h"

QueryResult *QueryOrchestrator::execute(
    PQLQuery* query,
    vector<shared_ptr<IEvaluatable>> evaluatables
    ) {
    QueryResult* finalResult = nullptr;
    QueryResult* currentResult;
    for (shared_ptr<IEvaluatable> ie : evaluatables) {
        currentResult = ie->evaluateOn();
        finalResult = coalescer.merge(finalResult, currentResult);
    }

    // Convert to QueryResult
    // TODO(KwanHW): WIP
    return new QueryResult;
}
