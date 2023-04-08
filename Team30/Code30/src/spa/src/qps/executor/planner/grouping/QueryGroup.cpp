#include "QueryGroup.h"

#include <utility>

#include "QueryGroupPlan.h"

using std::make_unique;

GroupClauseIndex QueryGroup::addEvaluatable(IEvaluatable *evaluatable) {
  int id = evaluatables.size();
  evaluatables.push_back(evaluatable);
  edgeList.push_back(unordered_set<GroupClauseIndex>());
  return id;
}

GroupClauseIndex QueryGroup::addEvaluatable(IEvaluatablePtr evaluatable) {
  ownedEvals.push_back(std::move(evaluatable));
  return addEvaluatable(ownedEvals.back().get());
}

void QueryGroup::linkEvaluatables(const GroupClauseIndex id1,
                                  const GroupClauseIndex id2) {
  if (id1 == id2) {
    return;
  }

  edgeList.at(id1).insert(id2);
  edgeList.at(id2).insert(id1);
}

void QueryGroup::addSelectable(const PQLSynonymName &synonym) {
  selectables.push_back(synonym);
}

int QueryGroup::getEvaluatableCount() const {
  return evaluatables.size();
}

IEvaluatable *QueryGroup::getEvaluatable(GroupClauseIndex evalId) const {
  return evaluatables[evalId];
}

const unordered_set<GroupClauseIndex> *QueryGroup::getRelated(
    const GroupClauseIndex evalId) const {
  return &edgeList[evalId];
}

QueryGroupPlanPtr QueryGroup::toPlan(vector<IEvaluatable *> newEvals,
                                     const ComplexityScore &score) {
  return make_unique<QueryGroupPlan>(newEvals,
                                     selectables,
                                     std::move(ownedEvals),
                                     score);
}
