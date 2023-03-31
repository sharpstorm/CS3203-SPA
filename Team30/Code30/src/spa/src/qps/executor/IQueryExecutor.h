#pragma once

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "../common/PQLQuery.h"
#include "qps/common/resulttable/SynonymResultTable.h"

class IQueryExecutor {
 public:
  virtual ~IQueryExecutor() = default;
  virtual SynonymResultTable* executeQuery(PQLQuery* query) = 0;
};
