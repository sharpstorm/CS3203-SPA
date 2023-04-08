#include <memory>
#include <vector>

#include "qps/common/IConstraint.h"
#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

using std::vector, std::unique_ptr, std::make_unique;

QueryExecutor::QueryExecutor(const PkbQueryHandler *pkbQH) :
    orchestrator(QueryOrchestrator(QueryLauncher(pkbQH))) {}

ProjectorResultTable *QueryExecutor::executeQuery(PQLQuery *query) {
  OverrideTable overrideTable;
  bool isBoolResult = query->isBooleanResult();

  bool areConstraintsResolved = query->resolveConstraints(&overrideTable);
  if (!areConstraintsResolved) {
    return new ProjectorResultTable(isBoolResult, false);
  }

  QueryPlanPtr plan = planner.getExecutionPlan(query, &overrideTable);
  // Query just have constraints
  if (plan->isEmpty()) {
    return new ProjectorResultTable(isBoolResult, true);
  }

  return orchestrator.execute(plan.get(), &overrideTable);
}
