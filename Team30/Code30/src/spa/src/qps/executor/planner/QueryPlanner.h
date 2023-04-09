#pragma once

#include "qps/common/PQLQuery.h"

#include "QueryPlan.h"
#include "qps/executor/planner/ordering/QueryGroupOrderer.h"
#include "qps/executor/planner/ordering/QueryClauseOrderer.h"

class QueryPlanner {
 public:
  QueryPlanPtr getExecutionPlan(const PQLQuery *query,
                                const OverrideTable *overrides) const;

 private:
  QueryGroupOrderer groupOrderer;
};
