#include "QueryGroupPlan.h"

#include <utility>

QueryGroupPlan::QueryGroupPlan(
    vector<IEvaluatable *> conditionalClauses,
    vector<PQLSynonymName> selectables,
    vector<IEvaluatablePtr> ownedEvals,
    const ComplexityScore &score,
    bool canEmpty) :
    conditionalClauses(conditionalClauses),
    selectables(selectables),
    ownedEvals(std::move(ownedEvals)),
    weightedComplexity(score),
    canEmpty(canEmpty) {}

const vector<IEvaluatable *> QueryGroupPlan::getConditionalClauses() const {
  return conditionalClauses;
}

bool QueryGroupPlan::isBooleanResult() const {
  return selectables.empty();
}

const PQLSynonymNameList *QueryGroupPlan::getSelectables() const {
  return &selectables;
}

bool QueryGroupPlan::canBeEmpty() const {
  return canEmpty;
}

ComplexityScore QueryGroupPlan::getComplexity() const {
  return weightedComplexity;
}
