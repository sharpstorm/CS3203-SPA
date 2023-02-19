#pragma once

#include <memory>
#include <queue>
#include <vector>
#include "qps/common/PQLQuery.h"

#include "QueryPlan.h"
#include "QueryPlannerIndex.h"

using std::unique_ptr, std::queue, std::vector;

class QueryPlanner {
 public:
  explicit QueryPlanner(PQLQuery* query);
  unique_ptr<QueryPlan> getExecutionPlan();

 private:
  PQLQuery* query;

  vector<IEvaluatableSPtr> evaluatables;
  vector<IEvaluatableSPtr> finalPlan;
  vector<QueryPlan::MergeStrategy> mergeStrategy;
  vector<bool> isEvaluatableLinked;

  QueryPlannerIndex planIndex;
  int currentPosition;
  bool isPlanBuilt;

  void buildPlan();
  void initState();
  void buildDependencyTree(PlanNodes* startingNodes);
  void flushIndependentClauses();

  void insertIntoQueue(queue<PlanNode>* target, PlanNodes* values);
};
