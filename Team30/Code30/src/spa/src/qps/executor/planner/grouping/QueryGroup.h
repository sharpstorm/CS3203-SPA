#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "qps/common/IEvaluatable.h"
#include "QueryGroupPlan.h"

using std::unique_ptr, std::vector, std::unordered_set;

class QueryGroup {
 public:
  QueryGroup(): canEmpty(false) {}
  explicit QueryGroup(bool canEmpty): canEmpty(canEmpty) {}

  int addEvaluatable(IEvaluatable* evaluatable);
  int addEvaluatable(IEvaluatablePtr evaluatable);
  void linkEvaluatables(int id1, int id2);
  void addSelectable(PQLSynonymName synonym);

  int getEvaluatableCount();
  IEvaluatable* getEvaluatable(int evalId);
  unordered_set<int>* getRelated(int evalId);

  QueryGroupPlanPtr toPlan(vector<IEvaluatable*> evaluatables);

 private:
  vector<IEvaluatable*> evaluatables;
  vector<unordered_set<int>> edgeList;
  vector<PQLSynonymName> selectables;
  vector<IEvaluatablePtr> ownedEvals;

  bool canEmpty;
};

typedef unique_ptr<QueryGroup> QueryGroupPtr;
