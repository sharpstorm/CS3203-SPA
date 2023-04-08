#include "QueryGroupPlan.h"

#include <utility>

QueryGroupPlan::QueryGroupPlan(IEvaluatableRefList conditionalClauses,
                               PQLSynonymNameList selectables,
                               const ComplexityScore &score) :
    conditionalClauses(conditionalClauses),
    selectables(selectables),
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

void QueryGroupPlan::adoptEvals(IEvaluatablePtrList *otherEvals) {
  for (auto &it : *otherEvals) {
    ownedEvals.push_back(std::move(it));
  }
}

const IEvaluatablePtrList &QueryGroupPlan::getOwnedEvals() {
  return ownedEvals;
}
