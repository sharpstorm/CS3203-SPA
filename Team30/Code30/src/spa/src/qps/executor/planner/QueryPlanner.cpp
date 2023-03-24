#include "QueryPlanner.h"

#include <utility>

#include "qps/common/IEvaluatable.h"
#include "qps/executor/planner/grouping/QueryGrouper.h"

QueryPlanPtr QueryPlanner::getExecutionPlan(PQLQuery *targetQuery,
                                            OverrideTable* overrides) {
  vector<QueryGroupPtr> groups = QueryGrouper(targetQuery).groupClauses();

  vector<QueryGroupPlanPtr> groupPlans(groups.size());
  for (int i = 0; i < groups.size(); i++) {
    QueryGroupPlanPtr groupOrder = clauseOrderer.orderClauses(groups[i].get(),
                                                              overrides);
    groupPlans[i] = std::move(groupOrder);
  }

  groupPlans = groupOrderer.orderGroups(std::move(groupPlans));

  return make_unique<QueryPlan>(std::move(groupPlans));
}
