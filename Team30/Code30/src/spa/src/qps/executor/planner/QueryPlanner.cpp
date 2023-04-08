#include "QueryPlanner.h"

#include <utility>
#include <memory>

#include "qps/executor/planner/grouping/QueryGrouper.h"

using std::make_unique;

QueryPlanPtr QueryPlanner::getExecutionPlan(const PQLQuery *targetQuery,
                                            const OverrideTable *overrides)
const {
  QueryGroupPtrList groups = QueryGrouper(targetQuery).groupClauses();
  QueryGroupPlanPtrList groupPlans(groups.size());

  for (int i = 0; i < groups.size(); i++) {
    QueryClauseOrderer orderer(groups[i].get(), overrides);
    QueryGroupPlanPtr groupOrder = orderer.orderClauses();
    groupPlans[i] = std::move(groupOrder);
  }

  groupPlans = groupOrderer.orderGroups(std::move(groupPlans));

  return make_unique<QueryPlan>(std::move(groupPlans));
}
