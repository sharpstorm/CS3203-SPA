#pragma once

#include <memory>
#include <vector>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::vector;

class QueryGroupPlan {
 public:
  QueryGroupPlan(vector<IEvaluatableSPtr> conditionalClauses,
                 vector<PQLSynonymName> selectables);
  vector<IEvaluatableSPtr> getConditionalClauses();
  bool isBooleanResult();
  vector<PQLSynonymName>* getSelectables();

 private:
  vector<IEvaluatableSPtr> conditionalClauses;
  vector<PQLSynonymName> selectables;
};

typedef unique_ptr<QueryGroupPlan> QueryGroupPlanPtr;