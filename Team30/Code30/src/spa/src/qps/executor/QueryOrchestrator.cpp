#include "QueryOrchestrator.h"

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
    launcher(launcher) {
}

PQLQueryResult *QueryOrchestrator::execute(PQLQuery* query,
                                           QueryPlan* plan) {
  PQLQueryResult* finalResult = nullptr;
  PQLQueryResult* currentResult;
  for (shared_ptr<IEvaluatable> ie : plan->getConditionalClauses()) {
    currentResult = launcher.execute(ie.get());
    finalResult = coalescer.merge(finalResult, currentResult);
  }

  if (plan->hasSelectClause() && shouldExecuteSelect(plan, finalResult)) {
    currentResult = launcher.execute(plan->getSelectClause().get());
    finalResult = coalescer.merge(finalResult, currentResult);
  }

  if (finalResult == nullptr) {
    return nullptr;
  }

  PQLQueryResult* filtered = finalResult
      ->filterResultTo(query->getResultVariable());
  delete(finalResult);

  return filtered;
}

bool QueryOrchestrator::shouldExecuteSelect(QueryPlan *plan,
                                            PQLQueryResult* state) {
  if (state == nullptr) {
    return true;
  }

  if (state->isStaticResult()) {
    return !state->getIsStaticFalse();
  }

  return !state->isEntityMapEmpty() || !state->isStatementMapEmpty();
}
