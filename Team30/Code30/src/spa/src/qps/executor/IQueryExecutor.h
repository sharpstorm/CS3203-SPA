#pragma once

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "../common/PQLQuery.h"
#include "qps/common/projector_table/ProjectorResultTable.h"

class IQueryExecutor {
 public:
  virtual ~IQueryExecutor() = default;
  virtual ProjectorResultTable* executeQuery(const PQLQuery* query) = 0;
};
