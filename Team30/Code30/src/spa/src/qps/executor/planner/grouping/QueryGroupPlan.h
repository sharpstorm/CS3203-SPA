#pragma once

#include <memory>
#include <vector>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::vector;

class QueryGroupPlan {
 public:
  QueryGroupPlan(vector<IEvaluatable*> conditionalClauses,
                 vector<PQLSynonymName> selectables,
                 vector<IEvaluatablePtr> ownedEvals,
                 const ComplexityScore &score,
                 bool canBeEmpty);
  vector<IEvaluatable*> getConditionalClauses();
  bool isBooleanResult();
  vector<PQLSynonymName>* getSelectables();
  bool canBeEmpty();
  ComplexityScore getComplexity();

 private:
  vector<IEvaluatable*> conditionalClauses;
  vector<PQLSynonymName> selectables;
  vector<IEvaluatablePtr> ownedEvals;
  bool canEmpty;
  ComplexityScore weightedComplexity;
};

typedef unique_ptr<QueryGroupPlan> QueryGroupPlanPtr;