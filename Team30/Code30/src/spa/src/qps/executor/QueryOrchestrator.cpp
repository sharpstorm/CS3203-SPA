#include <utility>
#include <memory>

#include "QueryOrchestrator.h"
#include "qps/common/projector_table/ProjectableGroupFactory.h"

using std::make_unique;

QueryOrchestrator::QueryOrchestrator(QueryLauncher launcher) :
    launcher(launcher) {
}

// Executes every group in the QueryPlan
ProjectableTable *QueryOrchestrator::execute(
    const QueryPlan *plan,
    const OverrideTable* overrideTable) const {
  if (plan->isEmpty()) {
    return new ProjectableTable(false);
  }

  QueryCache cache;
  ProjectableTable* resultTable = new ProjectableTable(true);
  for (int i = 0; i < plan->getGroupCount(); i++) {
    const QueryGroupPlan* targetGroup = plan->getGroup(i);
    PQLQueryResult* result = executeGroup(targetGroup, overrideTable, &cache);

    // If any of the result is empty, return FALSE / EmptyResultTable
    if (result->isFalse()) {
      delete resultTable;
      delete result;
      return new ProjectableTable(false);
    }

    if (targetGroup->isBooleanResult()) {
      delete result;
      continue;
    }

    const PQLSynonymNameList* selectables = targetGroup->getSelectables();
    ProjectableGroupPtr resultGroup =
        ProjectableGroupFactory::extractResults(result, selectables);
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
