#pragma once

#include <vector>
#include <memory>
#include "../common/IEvaluatable.h"

using std::vector, std::shared_ptr;

class QueryPlan {
 public:
  enum MergeStrategy {
    DISCARDING_MERGE,
    INNER_JOIN,
    SKIP,
  };

  QueryPlan(vector<shared_ptr<IEvaluatable>> conditionalClauses,
            vector<MergeStrategy> mergeStrategy);
  vector<shared_ptr<IEvaluatable>> getConditionalClauses();
  bool isEmpty();
  MergeStrategy strategyFor(int rightClausePosition);

 private:
  vector<shared_ptr<IEvaluatable>> conditionalClauses;
  vector<MergeStrategy> mergeStrategy;
};
