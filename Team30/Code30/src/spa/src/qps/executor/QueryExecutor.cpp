#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {
}

SynonymResultTable *QueryExecutor::executeQuery(PQLQuery* query) {
  // TODO(WeiXin): Move this if necessary
  OverrideTable* overrideTable = new OverrideTable();
  for (const auto& con : query->getConstraints()) {
    if (con->applyConstraint(query->getVarTable(), overrideTable)) {
      bool isBoolResult = query->getResultVariables()->empty();
      return new SynonymResultTable(isBoolResult, false);
    }
  }

  QueryPlanPtr plan = planner.getExecutionPlan(query);
  return orchestrator.execute(plan.get());
}
