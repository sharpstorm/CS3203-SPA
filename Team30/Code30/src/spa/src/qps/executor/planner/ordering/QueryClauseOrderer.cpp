#include "QueryClauseOrderer.h"

#include <memory>

using std::make_unique;

QueryGroupPlanPtr QueryClauseOrderer::orderClauses(QueryGroup *group) {
  vector<IEvaluatableSPtr> groupOrdering(group->getEvaluatableCount());
  BitField seenClauses(group->getEvaluatableCount());

  int curIndex = 0;
  queue<int> queuedClauses;
  queuedClauses.push(0);
  seenClauses.set(0);

  while (!queuedClauses.empty()) {
    int evalId = queuedClauses.front();
    queuedClauses.pop();

    IEvaluatableSPtr evaluatable = group->getEvaluatable(evalId);
    groupOrdering[curIndex] = evaluatable;
    curIndex++;

    unordered_set<int>* edges = group->getRelated(evalId);
    populateQueue(&queuedClauses, &seenClauses, edges);
  }

  return make_unique<QueryGroupPlan>(groupOrdering, group->getSelectables());
}

void QueryClauseOrderer::populateQueue(queue<int> *target,
                                       BitField *clauseDone,
                                       unordered_set<int> *edges) {
  for (auto it = edges->begin(); it != edges->end(); it++) {
    int otherClauseId = *it;
    if (clauseDone->isSet(otherClauseId)) {
      continue;
    }

    clauseDone->set(otherClauseId);
    target->push(otherClauseId);
  }
}
