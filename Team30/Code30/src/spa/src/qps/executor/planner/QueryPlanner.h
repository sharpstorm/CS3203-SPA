#pragma once

#include <memory>
#include <queue>
#include <vector>
#include "qps/common/PQLQuery.h"

#include "QueryPlan.h"
#include "qps/executor/planner/grouping/QueryGrouperIndex.h"
#include "qps/executor/planner/ordering/QueryGroupOrderer.h"
#include "qps/executor/planner/ordering/QueryClauseOrderer.h"

using std::unique_ptr, std::queue, std::vector;

class QueryPlanner {
 public:
  QueryPlanPtr getExecutionPlan(PQLQuery* query, OverrideTable* overrides);

 private:
  QueryClauseOrderer clauseOrderer;
  QueryGroupOrderer groupOrderer;
};
