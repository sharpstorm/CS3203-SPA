#include "QueryOrchestrator.h"

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
      if (finalResult == nullptr || !finalResult->isEntityMapEmpty()
          || !finalResult->isStatementMapEmpty()) {
        currentResult = launcher.execute(plan->getSelectClause().get());
        finalResult = coalescer.merge(finalResult, currentResult);
      }
    }

    return finalResult->resultFromVariable(query->getResultVariable());
}
