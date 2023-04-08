#pragma once

#include "../common/PQLQuery.h"
#include "qps/common/projector_table/ProjectableTable.h"

class IQueryExecutor {
 public:
  virtual ~IQueryExecutor() = default;
  virtual ProjectableTable* executeQuery(PQLQuery* query) const = 0;
};
