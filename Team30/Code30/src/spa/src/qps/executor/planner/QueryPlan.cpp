#include "QueryPlan.h"

#include <utility>

QueryPlan::QueryPlan(vector<QueryGroupPlanPtr> groups,
                     vector<ConstraintSPtr> cons):
    clauseGroups(std::move(groups)), constraints(cons) {}

int QueryPlan::getGroupCount() {
  return clauseGroups.size();
}

QueryGroupPlan *QueryPlan::getGroup(int groupId) {
  return clauseGroups.at(groupId).get();
}

bool QueryPlan::isEmpty() {
  return clauseGroups.empty();
}

bool QueryPlan::isBooleanQuery() {
  for (auto& group : clauseGroups) {
    vector<PQLSynonymName>* selectables = group->getSelectables();
    if (!selectables->empty()) {
      return false;
    }
  }

  return true;
}

vector<ConstraintSPtr> QueryPlan::getConstraints() {
  return constraints;
}

