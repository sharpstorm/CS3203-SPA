#pragma once

#include "IQueryExecutor.h"
#include "QueryOrchestrator.h"
#include "QueryPlanner.h"
#include "../common/PQLQuery.h"
#include "../common/QueryResult.h"

class QueryExecutor: public IQueryExecutor {
    QueryPlanner planner;
    QueryOrchestrator orchestrator;
 public:
    QueryResult* executeQuery(PQLQuery* query);
};
