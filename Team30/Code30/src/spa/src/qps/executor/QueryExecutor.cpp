#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {
}

SynonymResultTable *QueryExecutor::executeQuery(PQLQuery* query) {
  VariableTableProxyBuilder variableTableProxyBuilder(query->getVarTable());
  OverrideTable overrideTable;
  for (const auto& con : query->getConstraints()) {
    if (con->applyConstraint(&variableTableProxyBuilder, &overrideTable)) {
      bool isBoolResult = query->getResultVariables()->empty();
      return new SynonymResultTable(isBoolResult, false);
    }
  }
  variableTableProxyBuilder.build();

  QueryPlanPtr plan = planner.getExecutionPlan(query);
  return orchestrator.execute(plan.get());
}
