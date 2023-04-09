#pragma once

#include <queue>

#include "qps/executor/planner/grouping/QueryGroup.h"
#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "common/data_structs/BitField.h"
#include "ComparableContainer.h"

using std::priority_queue;

typedef int ClauseId;
typedef ComparableContainer<ComplexityScore, ClauseId> ComparableClause;

class QueryClauseOrderer {
 private:
  QueryGroup *group;
  const OverrideTable *overrides;

  ComparableClause toComparableClause(const IEvaluatable *eval,
                                      const ClauseId clause) const;
  ComplexityScore computeShortestPathFrom(
      const ClauseId start, IEvaluatableRefList *groupOrdering) const;
  void queueNeighboursOf(const ClauseId node,
                         BitField *seenClauses,
                         priority_queue<ComparableClause> *queuedClauses) const;

 public:
  explicit QueryClauseOrderer(QueryGroup *group,
                              const OverrideTable *overrides);
  QueryGroupPlanPtr orderClauses() const;
};
