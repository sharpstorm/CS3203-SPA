#include "QueryGroup.h"

int QueryGroup::addEvaluatable(IEvaluatableSPtr evaluatable) {
  int id = evaluatables.size();
  evaluatables.push_back(evaluatable);
  edgeList.push_back(vector<int>());
  return id;
}

void QueryGroup::linkEvaluatables(int id1, int id2) {
  edgeList.at(id1).push_back(id2);
  edgeList.at(id2).push_back(id1);
}
