#pragma once

#include <memory>

#include "Clause.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuery.h"

using std::unique_ptr;

class SuchThatClause: public Clause {
 public:
  ~SuchThatClause() override = default;
};

typedef unique_ptr<SuchThatClause> SuchThatClausePtr;
