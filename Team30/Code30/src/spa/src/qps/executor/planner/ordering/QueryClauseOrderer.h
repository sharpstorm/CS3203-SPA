#pragma once

#include <queue>
#include <vector>
#include <unordered_set>

#include "qps/executor/planner/grouping/QueryGroup.h"
#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "common/data_structs/BitField.h"
#include "ComparableContainer.h"

using std::priority_queue, std::vector, std::unordered_set;

typedef int ClauseId;
typedef ComparableContainer<ComplexityScore, ClauseId> ComparableClause;

class QueryClauseOrderer {
 public:
  QueryGroupPlanPtr orderClauses(QueryGroup* group, OverrideTable* overrides);
};
