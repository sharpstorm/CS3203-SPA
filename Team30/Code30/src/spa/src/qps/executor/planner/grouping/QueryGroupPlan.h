#pragma once

#include <memory>
#include <vector>

#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::vector;

class QueryGroupPlan {
 protected:
  const IEvaluatablePtrList &getOwnedEvals();

 public:
  QueryGroupPlan(IEvaluatableRefList conditionalClauses,
                 PQLSynonymNameList selectables,
                 const ComplexityScore &score);
  const IEvaluatableRefList getConditionalClauses() const;
  bool isBooleanResult() const;
  const PQLSynonymNameList *getSelectables() const;
  ComplexityScore getComplexity() const;
  void adoptEvals(IEvaluatablePtrList *otherEvals);

 private:
  IEvaluatableRefList conditionalClauses;
  PQLSynonymNameList selectables;
  IEvaluatablePtrList ownedEvals;
  ComplexityScore weightedComplexity;
};

typedef unique_ptr<QueryGroupPlan> QueryGroupPlanPtr;
typedef vector<QueryGroupPlanPtr> QueryGroupPlanPtrList;
