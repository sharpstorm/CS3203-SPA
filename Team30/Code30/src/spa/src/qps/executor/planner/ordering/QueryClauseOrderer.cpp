#include "QueryClauseOrderer.h"

#include <memory>

using std::make_unique;

QueryGroupPlanPtr QueryClauseOrderer::orderClauses(QueryGroup *group) {
  vector<IEvaluatableSPtr> groupOrdering(group->getEvaluatableCount());
  vector<bool> clauseDone(group->getEvaluatableCount());

  for (int i = 0; i < clauseDone.size(); i++) {
    clauseDone[i] = false;
  }

  int curIndex = 0;
  queue<int> queuedClauses;
  queuedClauses.push(0);

  while (!queuedClauses.empty()) {
    int evalId = queuedClauses.front();
    queuedClauses.pop();

    IEvaluatableSPtr evaluatable = group->getEvaluatable(evalId);
    groupOrdering[curIndex] = evaluatable;
    curIndex++;

    clauseDone[evalId] = true;
    unordered_set<int>* edges = group->getRelated(evalId);
    populateQueue(&queuedClauses, &clauseDone, edges);
  }

  return make_unique<QueryGroupPlan>(groupOrdering, group->getSelectables());
}

void QueryClauseOrderer::populateQueue(queue<int> *target,
                                       vector<bool> *clauseDone,
                                       unordered_set<int> *edges) {
  for (auto it = edges->begin(); it != edges->end(); it++) {
    int otherClauseId = *it;
    if (clauseDone->at(otherClauseId)) {
      continue;
    }

    target->push(otherClauseId);
  }
}
