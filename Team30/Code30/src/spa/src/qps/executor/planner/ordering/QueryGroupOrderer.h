#pragma once

#include "qps/executor/planner/QueryGroupPlan.h"

class QueryGroupOrderer {
 public:
  vector<QueryGroupPlanPtr> orderGroups(
      vector<QueryGroupPlanPtr> unordered);
};
