#pragma once

#include <memory>

#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "qps/common/IEvaluatable.h"
#include "qps/common/IConstraint.h"

using std::unique_ptr;

typedef int QueryPlanGroupId;

class QueryPlan {
 public:
  explicit QueryPlan(QueryGroupPlanPtrList groups);
  int getGroupCount() const;
  const QueryGroupPlan *getGroup(const QueryPlanGroupId groupId) const;
  bool isEmpty() const;

 private:
  QueryGroupPlanPtrList clauseGroups;
};

typedef unique_ptr<QueryPlan> QueryPlanPtr;
