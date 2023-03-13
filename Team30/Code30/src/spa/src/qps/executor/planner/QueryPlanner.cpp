#include "QueryPlanner.h"

#include <utility>

#include "qps/common/IEvaluatable.h"
#include "qps/executor/planner/grouping/QueryGrouper.h"

using std::move;

QueryPlanPtr QueryPlanner::getExecutionPlan(PQLQuery *targetQuery) {
  vector<QueryGroupPtr> groups = QueryGrouper(targetQuery).groupClauses();

  vector<QueryGroupPlanPtr> groupPlans(groups.size());
  for (int i = 0; i < groups.size(); i++) {
    QueryGroupPlanPtr groupOrder = clauseOrderer.orderClauses(groups[i].get());
    groupPlans[i] = move(groupOrder);
  }

  groupPlans = groupOrderer.orderGroups(move(groupPlans));

  return make_unique<QueryPlan>(move(groupPlans), targetQuery->getConstraints());
}
