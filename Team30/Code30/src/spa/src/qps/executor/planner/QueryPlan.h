#pragma once

#include <vector>
#include <memory>

#include "QueryGroupPlan.h"
#include "qps/common/IEvaluatable.h"
#include "qps/common/constraint/Constraint.h"

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
  vector<ConstraintSPtr> constraints;
};

typedef unique_ptr<QueryPlan> QueryPlanPtr;
