#pragma once

#include <vector>
#include <memory>
#include "qps/common/IEvaluatable.h"
#include "QueryGroupPlan.h"

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
