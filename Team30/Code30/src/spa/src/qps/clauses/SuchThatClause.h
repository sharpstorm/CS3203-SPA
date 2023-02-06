#pragma once

#include "Clause.h"
#include "ClauseArgument.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuery.h"

class SuchThatClause: public Clause {
 public:
  virtual ~SuchThatClause() = default;
};
