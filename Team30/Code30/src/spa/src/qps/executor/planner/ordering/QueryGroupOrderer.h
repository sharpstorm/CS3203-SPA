#pragma once

#include <vector>

#include "qps/executor/planner/QueryGroupPlan.h"

using std::vector;

class QueryGroupOrderer {
 public:
  vector<QueryGroupPlanPtr> orderGroups(
      vector<QueryGroupPlanPtr> unordered);
};
