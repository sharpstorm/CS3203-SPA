#pragma once

#include <vector>
#include <memory>
#include "../common/PQLQuery.h"
#include "../common/IEvaluatable.h"
#include "QueryPlan.h"

using std::vector;

class QueryPlanner {
 public:
  shared_ptr<QueryPlan> getExecutionPlan(PQLQuery* query);
};
