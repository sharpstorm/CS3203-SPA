#include <utility>
#include <memory>

#include "QueryOrchestrator.h"
#include "qps/common/projector_table/ProjectableGroupFactory.h"

using std::make_unique;

QueryOrchestrator::QueryOrchestrator(const QueryLauncher *launcher) :
    launcher(launcher),
    resultTable(make_unique<ProjectableTable>(true)) {}

// Executes every group in the QueryPlan
ProjectableTable *QueryOrchestrator::execute(
    const QueryPlan *plan,
    const OverrideTable *overrideTable) {
  // Default to True
  if (plan->isEmpty()) {
    ProjectableTable *resultTableRef = resultTable.get();
    resultTable.release();
    return resultTableRef;
  }

  QueryCache cache;
  for (int i = 0; i < plan->getGroupCount(); i++) {
    const QueryGroupPlan *targetGroup = plan->getGroup(i);
    bool shouldContinue = executeGroup(targetGroup, overrideTable, &cache);

    if (!shouldContinue) {
      break;
    }
  }
  ProjectableTable *resultTableRef = resultTable.get();
  resultTable.release();
  return resultTableRef;
}

bool QueryOrchestrator::executeGroup(const QueryGroupPlan *targetGroup,
                                     const OverrideTable *overrideTable,
                                     QueryCache *cache) {
  PQLQueryResult *result = executeClauses(targetGroup->getConditionalClauses(),
                                          overrideTable, cache);
  // If group is empty, return false result
  if (result->isFalse()) {
    resultTable = make_unique<ProjectableTable>(false);
    delete result;
    return false;
  }

  if (!targetGroup->isBooleanResult()) {
    extractProjectables(targetGroup, result);
  }

  delete result;
  return true;
}

// Executes each clause in the QueryGroupPlan
PQLQueryResult *QueryOrchestrator::executeClauses(
    const vector<IEvaluatable *> &executables,
    const OverrideTable *overrideTable,
    QueryCache *cache) const {
  PQLQueryResult *currentResult;
  PQLQueryResult *finalResult = nullptr;

  for (int i = 0; i < executables.size(); i++) {
    currentResult = launcher->execute(executables.at(i), overrideTable, cache);
    finalResult = ResultCoalescer(currentResult, finalResult).merge();
    if (finalResult == nullptr || finalResult->isFalse()) {
      delete finalResult;
      return new PQLQueryResult();
    }
    currentResult = nullptr;
  }

  return finalResult;
}

void QueryOrchestrator::extractProjectables(const QueryGroupPlan *targetGroup,
                                            PQLQueryResult *result) {
  const PQLSynonymNameList *selectables = targetGroup->getSelectables();
  ProjectableGroupPtr resultGroup =
      ProjectableGroupFactory::extractResults(result, selectables);
  resultTable->addResultGroup(std::move(resultGroup));
}
