#pragma once

#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../common/PQLQuery.h"

#include "QueryPlan.h"

using std::unique_ptr, std::queue, std::string;
using std::unordered_set, std::unordered_map;

typedef unordered_set<int> PlanNode;
typedef unordered_map<PQLSynonymName, PlanNode> PlanNodeMap;

class QueryPlanner {
 public:
  unique_ptr<QueryPlan> getExecutionPlan(PQLQuery* query);

 private:
  void insertIntoQueue(queue<int>* target, unordered_set<int>* values);
  PlanNode* getOrNull(PlanNodeMap* map, const string &key);
};
