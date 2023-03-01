#include "QueryGroupPlan.h"

QueryGroupPlan::QueryGroupPlan(
    vector<IEvaluatableSPtr> conditionalClauses,
    vector<PQLSynonymName> selectables):
    conditionalClauses(conditionalClauses),
    selectables(selectables) {}

vector<IEvaluatableSPtr> QueryGroupPlan::getConditionalClauses() {
  return conditionalClauses;
}

bool QueryGroupPlan::isBooleanResult() {
  return selectables.empty();
}

vector<PQLSynonymName> QueryGroupPlan::getSelectables() {
  return selectables;
}
