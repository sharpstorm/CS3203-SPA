#pragma once

#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "QueryPlanner.h"

class QueryExecutor: public IQueryExecutor {
  QueryPlanner planner;
  QueryOrchestrator orchestrator;
 public:
  explicit QueryExecutor(QueryOrchestrator orchestrator);
  PQLQueryResult* executeQuery(PQLQuery* query);
};
