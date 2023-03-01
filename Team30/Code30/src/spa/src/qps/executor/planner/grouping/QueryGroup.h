#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::vector, std::unordered_set;

class QueryGroup {
 public:
  int addEvaluatable(IEvaluatableSPtr evaluatable);
  void linkEvaluatables(int id1, int id2);
  void addSelectable(PQLSynonymName synonym);

  vector<PQLSynonymName> getSelectables();

  int getEvaluatableCount();
  IEvaluatableSPtr getEvaluatable(int evalId);
  unordered_set<int>* getRelated(int evalId);

 private:
  vector<IEvaluatableSPtr> evaluatables;
  vector<unordered_set<int>> edgeList;
  vector<PQLSynonymName> selectables;
};

typedef unique_ptr<QueryGroup> QueryGroupPtr;
