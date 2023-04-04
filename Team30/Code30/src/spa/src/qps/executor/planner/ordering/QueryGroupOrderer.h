#pragma once

#include <vector>

#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "ComparableContainer.h"

using std::vector;

typedef int GroupId;
typedef ComparableContainer<ComplexityScore, GroupId> ComparableGroup;

class QueryGroupOrderer {
 public:
  vector<QueryGroupPlanPtr> orderGroups(
      vector<QueryGroupPlanPtr> unordered) const;
};
