#pragma once

#include <memory>
#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "qps/executor/planner/QueryPlanner.h"

class QueryExecutor: public IQueryExecutor {
 private:
  QueryOrchestrator orchestrator;
  QueryPlanner planner;

 public:
  explicit QueryExecutor(shared_ptr<PkbQueryHandler> pkbQH);
  SynonymResultTable* executeQuery(PQLQuery* query);
};
