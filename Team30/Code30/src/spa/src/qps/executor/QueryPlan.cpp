#include "QueryPlan.h"

QueryPlan::QueryPlan(
    vector<shared_ptr<IEvaluatable>> conditionalClauses,
    vector<MergeStrategy> mergeStrategy):
    conditionalClauses(conditionalClauses), mergeStrategy(mergeStrategy) {}

bool QueryPlan::isEmpty() {
  return conditionalClauses.size() == 0;
}

QueryPlan::MergeStrategy QueryPlan::strategyFor(int rightClausePosition) {
  if (rightClausePosition < 1
      || rightClausePosition > conditionalClauses.size() - 1) {
    return SKIP;
  }
  return mergeStrategy[rightClausePosition - 1];
}

vector<shared_ptr<IEvaluatable>> QueryPlan::getConditionalClauses() {
  return conditionalClauses;
}
