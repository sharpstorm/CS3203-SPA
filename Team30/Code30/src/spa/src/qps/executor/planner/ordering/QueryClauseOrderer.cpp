#include "QueryClauseOrderer.h"

#include <memory>

using std::make_unique;

QueryGroupPlanPtr QueryClauseOrderer::orderClauses(QueryGroup *group,
                                                   OverrideTable* overrides) {
  int evalCount = group->getEvaluatableCount();
  int currentWeightedMin = INT_MAX;
  vector<IEvaluatable*> groupOrdering;

  vector<IEvaluatable*> tempOrdering(evalCount);
  for (int i = 0; i < evalCount; i++) {
    int runningWeight = 0;
    int curModifier = evalCount;
    int curIndex = 0;
    BitField seenClauses(evalCount);
    priority_queue<ComparableClause> queuedClauses;

    IEvaluatable* current = group->getEvaluatable(i);
    queuedClauses.push({current->getComplexityScore(overrides), i});
    seenClauses.set(i);
    while (!queuedClauses.empty()) {
      ComparableClause curClause = queuedClauses.top();
      queuedClauses.pop();

      IEvaluatable* evaluatable = group->getEvaluatable(curClause.getId());
      tempOrdering[curIndex] = evaluatable;
      runningWeight += curModifier * curClause.getComplexity();
      curIndex++;
      curModifier--;

      unordered_set<ClauseId>* edges = group->getRelated(curClause.getId());
      for (auto it = edges->begin(); it != edges->end(); it++) {
        ClauseId otherClauseId = *it;
        if (seenClauses.isSet(otherClauseId)) {
          continue;
        }

        IEvaluatable* otherEval = group->getEvaluatable(otherClauseId);
        seenClauses.set(otherClauseId);
        queuedClauses.push(
            {otherEval->getComplexityScore(overrides), otherClauseId});
      }
    }

    if (runningWeight < currentWeightedMin) {
      groupOrdering = tempOrdering;
      currentWeightedMin = runningWeight;
    }
  }

  return group->toPlan(groupOrdering);
}
