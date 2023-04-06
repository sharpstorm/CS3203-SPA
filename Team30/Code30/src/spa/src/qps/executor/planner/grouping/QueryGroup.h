#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "qps/common/IEvaluatable.h"
#include "QueryGroupPlan.h"

using std::unique_ptr, std::vector, std::unordered_set;

typedef int GroupClauseIndex;

class QueryGroup {
 public:
  QueryGroup() : canEmpty(false) {}
  explicit QueryGroup(bool canEmpty) : canEmpty(canEmpty) {}

  GroupClauseIndex addEvaluatable(IEvaluatable *evaluatable);
  GroupClauseIndex addEvaluatable(IEvaluatablePtr evaluatable);
  void linkEvaluatables(const GroupClauseIndex id1,
                        const GroupClauseIndex id2);
  void addSelectable(const PQLSynonymName &synonym);

  int getEvaluatableCount() const;
  IEvaluatable *getEvaluatable(GroupClauseIndex evalId) const;
  const unordered_set<GroupClauseIndex> *getRelated(
      const GroupClauseIndex evalId) const;

  QueryGroupPlanPtr toPlan(vector<IEvaluatable *> evaluatables,
                           const ComplexityScore &score);

 private:
  vector<IEvaluatable *> evaluatables;
  vector<unordered_set<GroupClauseIndex>> edgeList;
  vector<PQLSynonymName> selectables;
  vector<IEvaluatablePtr> ownedEvals;

  bool canEmpty;
};

typedef unique_ptr<QueryGroup> QueryGroupPtr;
