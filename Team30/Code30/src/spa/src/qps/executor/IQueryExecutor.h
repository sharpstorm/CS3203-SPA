#pragma once

#include "../common/PQLQueryResult.h"
#include "../common/PQLQuery.h"

class IQueryExecutor {
 public:
  PQLQueryResult* executeQuery(PQLQuery* query);
};
