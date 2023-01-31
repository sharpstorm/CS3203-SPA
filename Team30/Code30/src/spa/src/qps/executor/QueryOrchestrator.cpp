#pragma once

#include "QueryOrchestrator.h"

QueryResult *QueryOrchestrator::execute(PQLQuery* query,
                                        vector<IEvaluatable>* evaluatables) {
    IntermediateResultSet* finalResult;
    IntermediateResultSet* currentResult;
    for (IEvaluatable ie : *evaluatables) {
        currentResult = ie.evaluateOn();
        finalResult = coalescer.merge(finalResult, currentResult);
    }

    // Convert to QueryResult
    // TODO: WIP
    return new QueryResult;
}
