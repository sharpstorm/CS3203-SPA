#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "qps/common/IEvaluatable.h"
#include "QueryGroupPlan.h"

using std::unique_ptr, std::vector, std::unordered_set;

typedef int GroupClauseIndex;
typedef unordered_set<GroupClauseIndex> GroupClauseIndexSet;

class QueryGroup {
 public:
  QueryGroup() = default;

  GroupClauseIndex addEvaluatable(IEvaluatable *evaluatable);
  GroupClauseIndex addEvaluatable(IEvaluatablePtr evaluatable);
  void linkEvaluatables(const GroupClauseIndex id1,
                        const GroupClauseIndex id2);
  void addSelectable(const PQLSynonymName &synonym);

  int getEvaluatableCount() const;
  IEvaluatable *getEvaluatable(const GroupClauseIndex evalId) const;
  const GroupClauseIndexSet *getRelated(const GroupClauseIndex evalId) const;

  QueryGroupPlanPtr toPlan(const IEvaluatableRefList &evaluatables,
                           const ComplexityScore &score);

 private:
  IEvaluatableRefList evaluatables;
  vector<GroupClauseIndexSet> edgeList;
  PQLSynonymNameList selectables;
  IEvaluatablePtrList ownedEvals;
};

typedef unique_ptr<QueryGroup> QueryGroupPtr;
typedef vector<QueryGroupPtr> QueryGroupPtrList;
