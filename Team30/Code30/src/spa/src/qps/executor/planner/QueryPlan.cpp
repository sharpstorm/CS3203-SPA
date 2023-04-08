#include "QueryPlan.h"

#include <utility>

QueryPlan::QueryPlan(vector<QueryGroupPlanPtr> groups) :
    clauseGroups(std::move(groups)) {}

int QueryPlan::getGroupCount() const {
  return clauseGroups.size();
}

const QueryGroupPlan *QueryPlan::getGroup(const QueryPlanGroupId groupId)
const {
  return clauseGroups.at(groupId).get();
}

bool QueryPlan::isEmpty() const {
  return clauseGroups.empty();
}
