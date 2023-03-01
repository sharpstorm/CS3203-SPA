#pragma once

#include <vector>
#include <queue>
#include "qps/executor/planner/QueryGroupPlan.h"
#include "qps/common/PQLQuery.h"
#include "QueryGrouperIndex.h"
#include "QueryGroup.h"

using std::vector, std::queue;

class QueryGrouper {
 public:
  QueryGrouper(PQLQuery* query);
  vector<QueryGroupPtr> groupClauses();

 private:
  PQLQuery* query;
  QueryGrouperIndex groupIndex;
  vector<IEvaluatableSPtr> evaluatables;
  vector<int> groupClauseIdTable;

  void initIndex();
  void findGroups(vector<QueryGroupPtr>* result);
  QueryGroup* BFSFindDependents(int start);
  void queueClauses(queue<PlanNode> *target, PlanNodes *values,
                    QueryGroup *result, int parentClauseId);
};
