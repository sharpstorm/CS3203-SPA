#pragma once

#include <memory>
#include <queue>
#include "../common/PQLQuery.h"

#include "QueryPlan.h"

using std::unique_ptr, std::queue;

typedef unordered_set<int> PlanNode;
typedef unordered_map<PQLSynonymName, PlanNode> PlanNodeMap;

class QueryPlanner {
 public:
  unique_ptr<QueryPlan> getExecutionPlan(PQLQuery* query);

 private:
  void insertIntoQueue(queue<int>* target, unordered_set<int>* values);
  PlanNode* getOrNull(PlanNodeMap* map, const string &key);
};
