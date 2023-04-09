#include "QueryClauseOrderer.h"

#include <limits.h>
#include <memory>
#include <unordered_set>

using std::make_unique, std::unordered_set;

QueryClauseOrderer::QueryClauseOrderer(QueryGroup *group,
                                       const OverrideTable *overrides) :
    group(group), overrides(overrides) {}

ComparableClause QueryClauseOrderer::toComparableClause(
    const IEvaluatable *eval, const ClauseId clause) const {
  return {eval->getComplexityScore(overrides), clause};
}

QueryGroupPlanPtr QueryClauseOrderer::orderClauses() const {
  int evalCount = group->getEvaluatableCount();
  ComplexityScore currentWeightedMin = INT_MAX;
  IEvaluatableRefList groupOrdering;

  IEvaluatableRefList tempOrdering(evalCount);
  for (int i = 0; i < evalCount; i++) {
    ComplexityScore runningWeight = computeShortestPathFrom(i, &tempOrdering);
    if (runningWeight < currentWeightedMin) {
      groupOrdering = tempOrdering;
      currentWeightedMin = runningWeight;
    }
  }

  return group->toPlan(groupOrdering, currentWeightedMin);
}

ComplexityScore QueryClauseOrderer::computeShortestPathFrom(
    const ClauseId start, IEvaluatableRefList *groupOrdering) const {
  int evalCount = group->getEvaluatableCount();
  ComplexityScore runningWeight = 0;
  int curModifier = evalCount;
  int curIndex = 0;
  BitField seenClauses(evalCount);
  priority_queue<ComparableClause> queuedClauses;

  IEvaluatable *current = group->getEvaluatable(start);
  queuedClauses.push(toComparableClause(current, start));
  seenClauses.set(start);

  while (!queuedClauses.empty()) {
    ComparableClause curClause = queuedClauses.top();
    queuedClauses.pop();

    IEvaluatable *evaluatable = group->getEvaluatable(curClause.getData());
    (*groupOrdering)[curIndex] = evaluatable;
    runningWeight += curModifier * curClause.getMetric();
    curIndex++;
    curModifier--;

    queueNeighboursOf(curClause.getData(), &seenClauses, &queuedClauses);
  }

  return runningWeight;
}

void QueryClauseOrderer::queueNeighboursOf(
    const ClauseId node,
    BitField *seenClauses,
    priority_queue<ComparableClause> *queuedClauses) const {
  const GroupClauseIndexSet *edges = group->getRelated(node);
  for (auto it = edges->begin(); it != edges->end(); it++) {
    ClauseId otherClauseId = *it;
    if (seenClauses->isSet(otherClauseId)) {
      continue;
    }

    IEvaluatable *otherEval = group->getEvaluatable(otherClauseId);
    seenClauses->set(otherClauseId);
    queuedClauses->push(toComparableClause(otherEval, otherClauseId));
  }
}
