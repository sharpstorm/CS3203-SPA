#include "QueryOrchestrator.h"
#include <iostream>

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
        launcher(launcher) {
}

PQLQueryResult *QueryOrchestrator::execute(PQLQuery* query,
                                           shared_ptr<QueryPlan> plan) {
    PQLQueryResult* finalResult = nullptr;
    PQLQueryResult* currentResult;
    for (shared_ptr<IEvaluatable> ie : plan->getConditionalClauses()) {
        currentResult = launcher.execute(ie.get());
        finalResult = coalescer.merge(finalResult, currentResult);
    }

    if (plan->hasSelectClause()) {
      bool shouldMerge = finalResult == nullptr;
      if (finalResult != nullptr) {
        shouldMerge |= (finalResult->isStaticResult() && !finalResult->getIsStaticFalse());
        shouldMerge |= !finalResult->isStaticResult() && (!finalResult->isEntityMapEmpty()
            || !finalResult->isStatementMapEmpty());
      }

      if (shouldMerge) {
        currentResult = launcher.execute(plan->getSelectClause().get());
        finalResult = coalescer.merge(finalResult, currentResult);
      }
    }

    return finalResult->resultFromVariable(query->getResultVariable());
}
