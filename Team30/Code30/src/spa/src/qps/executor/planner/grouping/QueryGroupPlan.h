#pragma once

#include <memory>
#include <vector>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::vector;

class QueryGroupPlan {
 public:
  QueryGroupPlan(IEvaluatableRefList conditionalClauses,
                 vector<PQLSynonymName> selectables,
                 vector<IEvaluatablePtr> ownedEvals,
                 const ComplexityScore &score);
  const IEvaluatableRefList getConditionalClauses() const;
  bool isBooleanResult() const;
  const PQLSynonymNameList *getSelectables() const;
  ComplexityScore getComplexity() const;

 private:
  IEvaluatableRefList conditionalClauses;
  vector<PQLSynonymName> selectables;
  vector<IEvaluatablePtr> ownedEvals;
  ComplexityScore weightedComplexity;
};

typedef unique_ptr<QueryGroupPlan> QueryGroupPlanPtr;
typedef vector<QueryGroupPlanPtr> QueryGroupPlanPtrList;
