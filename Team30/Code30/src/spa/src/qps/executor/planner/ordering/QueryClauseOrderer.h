#pragma once

#include <queue>
#include <vector>
#include <unordered_set>

#include "qps/executor/planner/grouping/QueryGroup.h"
#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "common/data_structs/BitField.h"

using std::queue, std::vector, std::unordered_set;

typedef int ClauseId;

class QueryClauseOrderer {
 public:
  QueryGroupPlanPtr orderClauses(QueryGroup* group);

 private:
  void populateQueue(queue<ClauseId>* target,
                     BitField* clauseDone,
                     unordered_set<ClauseId>* edges);
};
