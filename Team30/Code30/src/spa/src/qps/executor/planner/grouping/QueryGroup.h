#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::vector, std::unordered_set;

class QueryGroup {
 public:
  QueryGroup(): canEmpty(false) {}
  explicit QueryGroup(bool canEmpty): canEmpty(canEmpty) {}
  int addEvaluatable(IEvaluatableSPtr evaluatable);
  void linkEvaluatables(int id1, int id2);
  void addSelectable(PQLSynonymName synonym);

  vector<PQLSynonymName> getSelectables();

  int getEvaluatableCount();
  IEvaluatableSPtr getEvaluatable(int evalId);
  unordered_set<int>* getRelated(int evalId);
  bool canBeEmpty();

 private:
  vector<IEvaluatableSPtr> evaluatables;
  vector<unordered_set<int>> edgeList;
  vector<PQLSynonymName> selectables;

  bool canEmpty;
};

typedef unique_ptr<QueryGroup> QueryGroupPtr;
