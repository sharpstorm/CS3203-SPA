#pragma once

#include "qps/common/QueryResult.h"
#include "qps/common/PQLQuery.h"
#include "Clause.h"
#include "ClauseArgument.h"

class SuchThatClause: public Clause {
  ClausePair args;
public:
  SuchThatClause(ClausePair args);
  virtual ~SuchThatClause() = default;
};
