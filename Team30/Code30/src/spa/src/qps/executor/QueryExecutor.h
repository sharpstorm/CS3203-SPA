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
  ProjectorResultTable *executeQuery(PQLQuery *query);
};
