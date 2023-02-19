#include "QueryPlan.h"

QueryPlan::QueryPlan(
    vector<IEvaluatableSPtr> conditionalClauses,
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

vector<IEvaluatableSPtr> QueryPlan::getConditionalClauses() {
  return conditionalClauses;
}
