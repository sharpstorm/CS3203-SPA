#pragma once

#include <memory>

#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuery.h"
#include "qps/clauses/Clause.h"

using std::unique_ptr;

class PatternClause: public Clause {
 public:
  virtual ~PatternClause() = default;
};

typedef unique_ptr<PatternClause> PatternClausePtr;
