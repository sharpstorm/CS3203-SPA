#pragma once

#include "IQueryExecutor.h"
#include "../common/PQLQuery.h"
#include "../common/QueryResult.h"

class QueryExecutor: public IQueryExecutor {
 public:
  QueryResult* executeQuery(PQLQuery* query);
};
