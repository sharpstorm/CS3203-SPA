#pragma once

#include <vector>
#include <memory>
#include "qps/common/Constraint.h"
#include "qps/common/IEvaluatable.h"
#include "QueryGroupPlan.h"

using std::vector, std::unique_ptr;

class QueryPlan {
 public:
  QueryPlan(vector<QueryGroupPlanPtr> groups, vector<ConstraintSPtr> cons);
  int getGroupCount();
  QueryGroupPlan* getGroup(int groupId);
  vector<ConstraintSPtr> getConstraints();
  bool isEmpty();
  bool isBooleanQuery();

 private:
  vector<QueryGroupPlanPtr> clauseGroups;
  vector<ConstraintSPtr> constraints;
};

typedef unique_ptr<QueryPlan> QueryPlanPtr;
