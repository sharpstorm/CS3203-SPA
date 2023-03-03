#include "QueryOrchestrator.h"

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
    launcher(launcher) {
}

// TODO(KwanHW): Fix execution to handle multiple groups
// Executes every group in the QueryPlan (OLD IMPLEMENTATION)
PQLQueryResult *QueryOrchestrator::execute(QueryPlan* plan) {
  if (plan->isEmpty()) {
    return new PQLQueryResult();
  }

  PQLQueryResult* finalResult;
  for (int i = 0; i < plan->getGroupCount(); i++) {
    QueryGroupPlan* targetGroup = plan->getGroup(i);
    PQLQueryResult* result = executeGroup(targetGroup);

    if (result->isFalse()) {
      return new PQLQueryResult();
    }

    if (targetGroup->isBooleanResult()) {
      delete result;
      continue;
    }

    finalResult = result;
  }
  return finalResult;
}

// Executes every group in the QueryPlan (NEW IMPLEMENTATION)
SynonymResultTable *QueryOrchestrator::execute(QueryPlan *plan,
                                           PQLQuerySynonymList* targetSyns) {
  if (plan->isEmpty()) {
    return new SynonymResultTable(targetSyns, false);
  }

  SynonymResultTable* resultTable = new SynonymResultTable(targetSyns, true);
  for (int i = 0; i < plan->getGroupCount(); i++) {
    QueryGroupPlan* targetGroup = plan->getGroup(i);
    PQLQueryResult* result = executeGroup(targetGroup);

    // If any of the result is empty, return FALSE / EmptyResultTable
    if (result->isFalse()) {
      return new SynonymResultTable(targetSyns, false);
    }

    if (targetGroup->isBooleanResult()) {
      delete result;
      continue;
    }

    resultTable->extractResults(result, targetGroup->getSelectables());
  }

  return resultTable;
}

// Executes each clause in the QueryGroupPlan
PQLQueryResult *QueryOrchestrator::executeGroup(QueryGroupPlan *plan) {
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

    if (i == 0) {
      delete finalResult;
      finalResult = currentResult;
      currentResult = nullptr;
      continue;
    }

    finalResult = coalescer.merge(currentResult, finalResult);
    if (finalResult->isFalse()) {
      delete finalResult;
      return new PQLQueryResult();
    }

    currentResult = nullptr;
  }

  return finalResult;
}

