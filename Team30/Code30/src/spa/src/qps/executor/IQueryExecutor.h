#pragma once

#include "../common/QueryResult.h"
#include "../common/PQLQuery.h"

class IQueryExecutor {
 public:
  QueryResult* executeQuery(PQLQuery* query);
};
