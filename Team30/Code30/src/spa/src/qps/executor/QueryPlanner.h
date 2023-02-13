#pragma once

#include <memory>
#include "../common/PQLQuery.h"

#include "QueryPlan.h"

using std::unique_ptr;

class QueryPlanner {
 public:
  unique_ptr<QueryPlan> getExecutionPlan(PQLQuery* query);
  bool isResultConditioned(PQLQuery* query);
};
