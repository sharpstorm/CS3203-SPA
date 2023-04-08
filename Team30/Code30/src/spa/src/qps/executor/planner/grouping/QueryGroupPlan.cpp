#include "QueryGroupPlan.h"

#include <utility>

QueryGroupPlan::QueryGroupPlan(
    IEvaluatableRefList conditionalClauses,
    vector<PQLSynonymName> selectables,
    vector<IEvaluatablePtr> ownedEvals,
    const ComplexityScore &score) :
    conditionalClauses(conditionalClauses),
    selectables(selectables),
    ownedEvals(std::move(ownedEvals)),
    weightedComplexity(score) {}

const IEvaluatableRefList QueryGroupPlan::getConditionalClauses() const {
  return conditionalClauses;
}

bool QueryGroupPlan::isBooleanResult() const {
  return selectables.empty();
}

const PQLSynonymNameList *QueryGroupPlan::getSelectables() const {
  return &selectables;
}

ComplexityScore QueryGroupPlan::getComplexity() const {
  return weightedComplexity;
}
