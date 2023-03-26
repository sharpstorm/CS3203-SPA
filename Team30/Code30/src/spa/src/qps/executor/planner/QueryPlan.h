#pragma once

#include <vector>
#include <memory>

#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "qps/common/IEvaluatable.h"
#include "qps/constraints/Constraint.h"

using std::vector, std::unique_ptr;

class QueryPlan {
 public:
  explicit QueryPlan(vector<QueryGroupPlanPtr> groups);
  int getGroupCount();
  QueryGroupPlan* getGroup(int groupId);
  bool isEmpty();
  bool isBooleanQuery();

 private:
  vector<QueryGroupPlanPtr> clauseGroups;
};

typedef unique_ptr<QueryPlan> QueryPlanPtr;
