#pragma once

#include <memory>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/executor/QueryExecutorAgent.h"

using std::unique_ptr;

typedef int ComplexityScore;

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) = 0;
  virtual PQLSynonymNameList getUsedSynonyms() = 0;
  virtual ComplexityScore getComplexityScore(const OverrideTable *table) = 0;
};

typedef unique_ptr<IEvaluatable> IEvaluatablePtr;
