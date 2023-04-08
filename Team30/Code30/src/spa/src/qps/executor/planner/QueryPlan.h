#pragma once

#include <vector>
#include <memory>

#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "qps/common/IEvaluatable.h"
#include "qps/common/IConstraint.h"

using std::vector, std::unique_ptr;

typedef int QueryPlanGroupId;

class QueryPlan {
 public:
  explicit QueryPlan(vector<QueryGroupPlanPtr> groups);
  int getGroupCount() const;
  const QueryGroupPlan *getGroup(const QueryPlanGroupId groupId) const;
  bool isEmpty() const;

 private:
  vector<QueryGroupPlanPtr> clauseGroups;
};

typedef unique_ptr<QueryPlan> QueryPlanPtr;
