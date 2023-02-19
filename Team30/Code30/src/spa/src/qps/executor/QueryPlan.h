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

  QueryPlan(vector<IEvaluatableSPtr> conditionalClauses,
            vector<MergeStrategy> mergeStrategy);
  vector<IEvaluatableSPtr> getConditionalClauses();
  bool isEmpty();
  MergeStrategy strategyFor(int rightClausePosition);

 private:
  vector<IEvaluatableSPtr> conditionalClauses;
  vector<MergeStrategy> mergeStrategy;
};
