#pragma once

#include <vector>
#include <memory>
#include "../common/IEvaluatable.h"

using std::vector, std::shared_ptr;

class QueryPlan {
 public:
  QueryPlan(
      shared_ptr<IEvaluatable> selectClause,
      vector<shared_ptr<IEvaluatable>> conditionalClauses);
  explicit QueryPlan(vector<shared_ptr<IEvaluatable>> conditionalClauses);
  shared_ptr<IEvaluatable> getSelectClause();
  vector<shared_ptr<IEvaluatable>> getConditionalClauses();
  bool hasSelectClause();

 private:
  shared_ptr<IEvaluatable> selectClause;
  vector<shared_ptr<IEvaluatable>> conditionalClauses;
};
