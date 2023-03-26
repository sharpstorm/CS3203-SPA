#include "QueryClauseOrderer.h"

#include <memory>

using std::make_unique;

QueryGroupPlanPtr QueryClauseOrderer::orderClauses(QueryGroup *group) {
  vector<IEvaluatable*> groupOrdering(group->getEvaluatableCount());
  BitField seenClauses(group->getEvaluatableCount());

  int curIndex = 0;
  queue<ClauseId> queuedClauses;
  queuedClauses.push(0);
  seenClauses.set(0);

  while (!queuedClauses.empty()) {
    ClauseId evalId = queuedClauses.front();
    queuedClauses.pop();

    IEvaluatable* evaluatable = group->getEvaluatable(evalId);
    groupOrdering[curIndex] = evaluatable;
    curIndex++;

    unordered_set<ClauseId>* edges = group->getRelated(evalId);
    populateQueue(&queuedClauses, &seenClauses, edges);
  }

  return group->toPlan(groupOrdering);
}

void QueryClauseOrderer::populateQueue(queue<ClauseId> *target,
                                       BitField *clauseDone,
                                       unordered_set<ClauseId> *edges) {
  for (auto it = edges->begin(); it != edges->end(); it++) {
    ClauseId otherClauseId = *it;
    if (clauseDone->isSet(otherClauseId)) {
      continue;
    }

    clauseDone->set(otherClauseId);
    target->push(otherClauseId);
  }
}
