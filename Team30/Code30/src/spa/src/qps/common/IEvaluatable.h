#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PQLQueryResult.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/constraints/Constraint.h"
#include "qps/executor/QueryExecutorAgent.h"

using std::unique_ptr, std::vector, std::string;

typedef vector<PQLSynonymName> SynonymList;
typedef int ComplexityScore;

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) = 0;
  virtual SynonymList getUsedSynonyms() = 0;
  virtual ComplexityScore getComplexityScore(const OverrideTable *table) = 0;
};

typedef unique_ptr<IEvaluatable> IEvaluatablePtr;
