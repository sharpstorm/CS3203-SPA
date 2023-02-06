#pragma once

#include "../common/PQLQueryResult.h"
#include "../common/PQLQuery.h"

class IQueryExecutor {
 public:
  virtual ~IQueryExecutor() = default;
  virtual PQLQueryResult* executeQuery(PQLQuery* query) = 0;
};
