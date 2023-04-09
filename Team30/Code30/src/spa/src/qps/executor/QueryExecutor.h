#pragma once

#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "qps/executor/planner/QueryPlanner.h"

class QueryExecutor : public IQueryExecutor {
 private:
  QueryLauncher launcher;
  QueryPlanner planner;

 public:
  explicit QueryExecutor(const PkbQueryHandler *pkbQH);
  ProjectableTable *executeQuery(PQLQuery *query) const;
};
