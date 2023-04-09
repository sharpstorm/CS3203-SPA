#pragma once

#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "ComparableContainer.h"

typedef int GroupId;
typedef ComparableContainer<ComplexityScore, GroupId> ComparableGroup;

class QueryGroupOrderer {
 public:
  QueryGroupPlanPtrList orderGroups(QueryGroupPlanPtrList unordered) const;
};
