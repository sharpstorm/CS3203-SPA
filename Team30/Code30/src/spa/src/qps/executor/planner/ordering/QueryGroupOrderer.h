#pragma once

#include <vector>

#include "qps/executor/planner/grouping/QueryGroupPlan.h"

using std::vector;

class QueryGroupOrderer {
 public:
  vector<QueryGroupPlanPtr> orderGroups(
      vector<QueryGroupPlanPtr> unordered);
};
