#include "QueryGroupPlan.h"

QueryGroupPlan::QueryGroupPlan(
    vector<IEvaluatableSPtr> conditionalClauses,
    vector<PQLSynonymName> selectables,
    bool canEmpty) :
    conditionalClauses(conditionalClauses),
    selectables(selectables),
    canEmpty(canEmpty)
    {}

vector<IEvaluatableSPtr> QueryGroupPlan::getConditionalClauses() {
  return conditionalClauses;
}

bool QueryGroupPlan::isBooleanResult() {
  return selectables.empty();
}

vector<PQLSynonymName>* QueryGroupPlan::getSelectables() {
  return &selectables;
}

bool QueryGroupPlan::canBeEmpty() {
  return canEmpty;
}
