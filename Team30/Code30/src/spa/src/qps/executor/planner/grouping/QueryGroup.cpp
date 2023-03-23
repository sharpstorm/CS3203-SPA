#include "QueryGroup.h"
#include "QueryGroupPlan.h"

using std::make_unique;

int QueryGroup::addEvaluatable(IEvaluatable* evaluatable) {
  int id = evaluatables.size();
  evaluatables.push_back(evaluatable);
  edgeList.push_back(unordered_set<int>());
  return id;
}

int QueryGroup::addEvaluatable(IEvaluatablePtr evaluatable) {
  ownedEvals.push_back(std::move(evaluatable));
  return addEvaluatable(ownedEvals.back().get());
}

void QueryGroup::linkEvaluatables(int id1, int id2) {
  if (id1 == id2) {
    return;
  }

  edgeList.at(id1).insert(id2);
  edgeList.at(id2).insert(id1);
}

void QueryGroup::addSelectable(PQLSynonymName synonym) {
  selectables.push_back(synonym);
}

int QueryGroup::getEvaluatableCount() {
  return evaluatables.size();
}

IEvaluatable* QueryGroup::getEvaluatable(int evalId) {
  return evaluatables[evalId];
}

unordered_set<int> *QueryGroup::getRelated(int evalId) {
  return &edgeList[evalId];
}

QueryGroupPlanPtr QueryGroup::toPlan(vector<IEvaluatable *> newEvals) {
  return make_unique<QueryGroupPlan>(newEvals,
                                     selectables,
                                     std::move(ownedEvals),
                                     canEmpty);
}
