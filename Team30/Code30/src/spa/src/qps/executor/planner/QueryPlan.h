#pragma once

#include <vector>
#include <memory>

#include "QueryGroupPlan.h"
#include "qps/common/IEvaluatable.h"
#include "qps/common/constraint/Constraint.h"

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
