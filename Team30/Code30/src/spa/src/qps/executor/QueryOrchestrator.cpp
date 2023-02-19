#include "QueryOrchestrator.h"

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
    launcher(launcher) {
}

PQLQueryResult *QueryOrchestrator::execute(QueryPlan* plan) {
  if (plan->isEmpty()) {
    return new PQLQueryResult();
  }

  vector<IEvaluatableSPtr> executables = plan->getConditionalClauses();
  PQLQueryResult* currentResult;
  PQLQueryResult* finalResult = nullptr;

  for (int i = 0; i < executables.size(); i++) {
    currentResult = launcher.execute(executables[i].get());
    if (currentResult->isFalse()) {
      delete currentResult;
      delete finalResult;
      return new PQLQueryResult();
    }

    auto strategy = plan->strategyFor(i);
    if (strategy == QueryPlan::INNER_JOIN) {
      finalResult = coalescer.merge(currentResult, finalResult);
      if (finalResult->isFalse()) {
        delete finalResult;
        return new PQLQueryResult();
      }

    } else {
      // Discarding and skip, we do take right
      // isEmpty guarantee that for discards, left is not empty / false
      delete finalResult;
      finalResult = currentResult;
    }
    currentResult = nullptr;
  }

  return finalResult;
}
