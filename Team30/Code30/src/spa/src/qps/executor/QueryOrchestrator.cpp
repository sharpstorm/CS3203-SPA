#include <utility>
#include <memory>

#include "QueryOrchestrator.h"
#include "qps/common/resulttable/ResultGroupFactory.h"
#include "QueryCache.h"

using std::make_unique;

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
    launcher(launcher) {
}

// Executes every group in the QueryPlan
SynonymResultTable *QueryOrchestrator::execute(QueryPlan *plan,
                                               OverrideTable* overrideTable) {
  bool isBool = plan->isBooleanQuery();
  if (plan->isEmpty()) {
    return new SynonymResultTable(isBool, false);
  }

  QueryCache cache;
  SynonymResultTable* resultTable = new SynonymResultTable(isBool, true);
  for (int i = 0; i < plan->getGroupCount(); i++) {
    QueryGroupPlan* targetGroup = plan->getGroup(i);
    PQLQueryResult* result = executeGroup(targetGroup, overrideTable, &cache);

    // If any of the result is empty, return FALSE / EmptyResultTable
    if (result->isFalse() && !targetGroup->canBeEmpty()) {
      delete resultTable;
      delete result;
      return new SynonymResultTable(isBool, false);
    } else if (result->isFalse() && targetGroup->canBeEmpty()) {
      delete result;
      continue;
    }

    if (targetGroup->isBooleanResult()) {
      delete result;
      continue;
    }

    vector<PQLSynonymName>* selectables = targetGroup->getSelectables();
    ResultGroupPtr resultGroup =
        ResultGroupFactory::extractResults(result, selectables);
    resultTable->addResultGroup(std::move(resultGroup));
    delete result;
  }

  return resultTable;
}

// Executes each clause in the QueryGroupPlan
PQLQueryResult *QueryOrchestrator::executeGroup(
    QueryGroupPlan *plan, OverrideTable* overrideTable,
    QueryCache *cache) {
  vector<IEvaluatable*> executables = plan->getConditionalClauses();
  PQLQueryResult* currentResult;
  PQLQueryResult* finalResult = nullptr;

  for (int i = 0; i < executables.size(); i++) {
    currentResult = launcher.execute(executables[i], overrideTable, cache);
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
