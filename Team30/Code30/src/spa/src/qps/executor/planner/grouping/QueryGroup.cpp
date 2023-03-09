#include "QueryGroup.h"

int QueryGroup::addEvaluatable(IEvaluatableSPtr evaluatable) {
  int id = evaluatables.size();
  evaluatables.push_back(evaluatable);
  edgeList.push_back(unordered_set<int>());
  return id;
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

vector<PQLSynonymName> QueryGroup::getSelectables() {
  return selectables;
}

int QueryGroup::getEvaluatableCount() {
  return evaluatables.size();
}

IEvaluatableSPtr QueryGroup::getEvaluatable(int evalId) {
  return evaluatables[evalId];
}

unordered_set<int> *QueryGroup::getRelated(int evalId) {
  return &edgeList[evalId];
}