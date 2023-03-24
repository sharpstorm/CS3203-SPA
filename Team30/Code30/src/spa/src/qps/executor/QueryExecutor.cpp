#include <memory>
#include <vector>

#include "qps/constraints/Constraint.h"
#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector, std::unique_ptr, std::make_unique;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) { }

SynonymResultTable *QueryExecutor::executeQuery(PQLQuery* query) {
  OverrideTablePtr overrideTable = make_unique<OverrideTable>();
  bool isBoolResult = query->isBooleanResult();

  bool areConstraintsResolved =
      resolveConstraints(query, overrideTable.get());
  if (!areConstraintsResolved) {
    return new SynonymResultTable(isBoolResult, false);
  }

  QueryPlanPtr plan = planner.getExecutionPlan(query);
  // Query just have constraints
  if (plan->isEmpty()) {
    return new SynonymResultTable(isBoolResult, true);
  }

  return orchestrator.execute(plan.get(), overrideTable.get());
}

bool QueryExecutor::resolveConstraints(PQLQuery* query,
                                       OverrideTable* overrideTable) {
  SynonymProxyBuilderPtr synProxyBuilder = make_unique<SynonymProxyBuilder>(
      query->getVarTable());

  for (const auto& con : query->getConstraints()) {
    if (!con->applyConstraint(synProxyBuilder.get(), overrideTable)) {
      return false;
    }
  }
  synProxyBuilder->build();
  return synProxyBuilder->resolveOverrideMerging(overrideTable);
}
