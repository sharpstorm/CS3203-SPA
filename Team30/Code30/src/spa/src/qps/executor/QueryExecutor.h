#pragma once

#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "qps/executor/planner/QueryPlanner.h"

class QueryExecutor : public IQueryExecutor {
 private:
  QueryOrchestrator orchestrator;
  QueryPlanner planner;

 public:
  explicit QueryExecutor(const PkbQueryHandler *pkbQH);
  ProjectorResultTable *executeQuery(const PQLQuery *query);

 private:
  bool resolveConstraints(const PQLQuery *query,
                          OverrideTable *overrideTable) const;
};
