#include <memory>
#include <vector>

#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector, std::unique_ptr, std::make_unique;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH): pkbQueryHandler(pkbQH),
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) { }

SynonymResultTable *QueryExecutor::executeQuery(PQLQuery* query) {
  OverrideTablePtr overrideTable = make_unique<OverrideTable>();
  bool isBoolResult = query->getResultVariables()->empty();
  for (const auto& con : query->getConstraints()) {
    if (!con->applyConstraint(
        query->getVarTable(), overrideTable.get(), pkbQueryHandler)) {
      return new SynonymResultTable(isBoolResult, false);
    }
  }

  QueryPlanPtr plan = planner.getExecutionPlan(query);

  // Query just have constraints
  if (plan->isEmpty()) {
    return new SynonymResultTable(isBoolResult, true);
  }

  return orchestrator.execute(plan.get(), overrideTable.get());
}
