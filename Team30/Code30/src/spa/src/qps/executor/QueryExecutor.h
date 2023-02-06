#pragma once

#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "QueryPlanner.h"

class QueryExecutor: public IQueryExecutor {
  QueryPlanner planner;
  QueryOrchestrator orchestrator;
 public:
  QueryExecutor(shared_ptr<PkbQueryHandler> pkbQH);
  PQLQueryResult* executeQuery(PQLQuery* query);
};
