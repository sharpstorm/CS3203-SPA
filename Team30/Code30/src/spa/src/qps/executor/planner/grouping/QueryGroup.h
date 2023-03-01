#pragma once

#include <memory>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr;

class QueryGroup {
 public:
  int addEvaluatable(IEvaluatableSPtr evaluatable);
  void linkEvaluatables(int id1, int id2);

 private:
  vector<IEvaluatableSPtr> evaluatables;
  vector<vector<int>> edgeList;
};

typedef unique_ptr<QueryGroup> QueryGroupPtr;
