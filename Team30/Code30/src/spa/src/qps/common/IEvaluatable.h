#pragma once

#include <memory>
#include <vector>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/executor/QueryExecutorAgent.h"
#include "qps/common/synonym/PQLTypes.h"

using std::unique_ptr, std::vector;

typedef int ComplexityScore;

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const = 0;
  virtual PQLSynonymNameList getUsedSynonyms() const = 0;
  virtual ComplexityScore getComplexityScore(
      const OverrideTable *table) const = 0;
};

typedef unique_ptr<IEvaluatable> IEvaluatablePtr;
typedef vector<IEvaluatable *> IEvaluatableRefList;
typedef vector<IEvaluatablePtr> IEvaluatablePtrList;

