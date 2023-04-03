#pragma once

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "../common/PQLQuery.h"
#include "qps/common/projector_table/ProjectorResultTable.h"

class IQueryExecutor {
 public:
  virtual ~IQueryExecutor() = default;
  virtual ProjectorResultTable* executeQuery(PQLQuery* query) = 0;
};
