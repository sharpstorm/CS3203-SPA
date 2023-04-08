#include "qps/common/IConstraint.h"
#include "QueryExecutor.h"
#include "qps/common/IEvaluatable.h"

QueryExecutor::QueryExecutor(const PkbQueryHandler *pkbQH) :
    launcher(pkbQH) {}

ProjectableTable *QueryExecutor::executeQuery(PQLQuery *query) const {
  OverrideTable overrideTable;

  bool areConstraintsResolved = query->resolveConstraints(&overrideTable);
  if (!areConstraintsResolved) {
    return new ProjectableTable(false);
  }

  QueryPlanPtr plan = planner.getExecutionPlan(query, &overrideTable);
  QueryOrchestrator orchestrator(&launcher);
  return orchestrator.execute(plan.get(), &overrideTable);
}
