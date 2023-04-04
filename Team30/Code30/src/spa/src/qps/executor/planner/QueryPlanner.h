#pragma once

#include <memory>
#include "qps/common/PQLQuery.h"

#include "QueryPlan.h"
#include "qps/executor/planner/ordering/QueryGroupOrderer.h"
#include "qps/executor/planner/ordering/QueryClauseOrderer.h"

using std::unique_ptr, std::vector;

class QueryPlanner {
 public:
  QueryPlanPtr getExecutionPlan(const PQLQuery *query,
                                const OverrideTable *overrides);

 private:
  QueryClauseOrderer clauseOrderer;
  QueryGroupOrderer groupOrderer;
};
