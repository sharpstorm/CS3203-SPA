#include <memory>
#include <vector>

#include "qps/constraints/Constraint.h"
#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector, std::unique_ptr, std::make_unique;

QueryExecutor::QueryExecutor(PkbQueryHandler* pkbQH):
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) { }

SynonymResultTable *QueryExecutor::executeQuery(PQLQuery* query) {
  OverrideTable overrideTable;

  bool isBoolResult = query->isBooleanResult();

  bool areConstraintsResolved =
      resolveConstraints(query, &overrideTable);
  if (!areConstraintsResolved) {
    return new SynonymResultTable(isBoolResult, false);
  }

  QueryPlanPtr plan = planner.getExecutionPlan(query, &overrideTable);
  // Query just have constraints
  if (plan->isEmpty()) {
    return new SynonymResultTable(isBoolResult, true);
  }

  return orchestrator.execute(plan.get(), &overrideTable);
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
