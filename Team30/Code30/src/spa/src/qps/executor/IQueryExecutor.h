#pragma once

#include "../common/PQLQueryResult.h"
#include "../common/PQLQuery.h"
#include "SynonymResultTable.h"

class IQueryExecutor {
 public:
  virtual ~IQueryExecutor() = default;
  virtual SynonymResultTable* executeQuery(PQLQuery* query) = 0;
};
