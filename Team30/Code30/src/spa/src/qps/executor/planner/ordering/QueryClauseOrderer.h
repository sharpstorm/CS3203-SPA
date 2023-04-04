#pragma once

#include "qps/executor/planner/grouping/QueryGroup.h"
#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "common/data_structs/BitField.h"
#include "ComparableContainer.h"

typedef int ClauseId;
typedef ComparableContainer<ComplexityScore, ClauseId> ComparableClause;

class QueryClauseOrderer {
 public:
  QueryGroupPlanPtr orderClauses(QueryGroup* group,
                                 const OverrideTable* overrides) const;
};
