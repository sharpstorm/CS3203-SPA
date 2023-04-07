#include <utility>
#include <memory>

#include "QueryOrchestrator.h"
#include "qps/common/projector_table/ProjectorResultFactory.h"

using std::make_unique;

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
    launcher(launcher) {
}

// Executes every group in the QueryPlan
ProjectorResultTable *QueryOrchestrator::execute(
    const QueryPlan *plan,
    const OverrideTable* overrideTable) const {
  bool isBool = plan->isBooleanQuery();
  if (plan->isEmpty()) {
    return new ProjectorResultTable(isBool, false);
  }

  QueryCache cache;
  ProjectorResultTable* resultTable = new ProjectorResultTable(isBool, true);
  for (int i = 0; i < plan->getGroupCount(); i++) {
    const QueryGroupPlan* targetGroup = plan->getGroup(i);
    PQLQueryResult* result = executeGroup(targetGroup, overrideTable, &cache);

    // If any of the result is empty, return FALSE / EmptyResultTable
    if (result->isFalse()) {
      delete resultTable;
      delete result;
      return new ProjectorResultTable(isBool, false);
    }

    if (targetGroup->isBooleanResult()) {
      delete result;
      continue;
    }

    const PQLSynonymNameList* selectables = targetGroup->getSelectables();
    ResultGroupPtr resultGroup =
        ProjectorResultFactory::extractResults(result, selectables);
    resultTable->addResultGroup(std::move(resultGroup));
    delete result;
  }

  return resultTable;
}

// Executes each clause in the QueryGroupPlan
PQLQueryResult *QueryOrchestrator::executeGroup(
    const QueryGroupPlan *plan,
    const OverrideTable* overrideTable,
    QueryCache *cache) const {
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

    finalResult = ResultCoalescer(currentResult, finalResult).merge();
    if (finalResult->isFalse()) {
      delete finalResult;
      return new PQLQueryResult();
    }

    currentResult = nullptr;
  }

  return finalResult;
}
