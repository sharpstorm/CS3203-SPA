#pragma once

#include <memory>
#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "QueryPlanner.h"

class QueryExecutor: public IQueryExecutor {
  QueryPlanner planner;
  QueryOrchestrator orchestrator;
 public:
  explicit QueryExecutor(shared_ptr<PkbQueryHandler> pkbQH);
  PQLQueryResult* executeQuery(PQLQuery* query);
};
