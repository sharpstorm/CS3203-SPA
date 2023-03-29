#include "QueryGroupPlan.h"

#include <utility>

QueryGroupPlan::QueryGroupPlan(
    vector<IEvaluatable*> conditionalClauses,
    vector<PQLSynonymName> selectables,
    vector<IEvaluatablePtr> ownedEvals,
    const ComplexityScore &score,
    bool canEmpty) :
    conditionalClauses(conditionalClauses),
    selectables(selectables),
    ownedEvals(std::move(ownedEvals)),
    weightedComplexity(score),
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

ComplexityScore QueryGroupPlan::getComplexity() {
  return weightedComplexity;
}
