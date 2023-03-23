#include "QueryGroupPlan.h"

QueryGroupPlan::QueryGroupPlan(
    vector<IEvaluatable*> conditionalClauses,
    vector<PQLSynonymName> selectables,
    vector<IEvaluatablePtr> ownedEvals,
    bool canEmpty) :
    conditionalClauses(conditionalClauses),
    selectables(selectables),
    ownedEvals(std::move(ownedEvals)),
    canEmpty(canEmpty)
{}

vector<IEvaluatable*> QueryGroupPlan::getConditionalClauses() {
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
