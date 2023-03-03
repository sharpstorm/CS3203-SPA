#pragma once

#include <string>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/SuchThatClause.h"
#include "common/Types.h"
#include "qps/clauses/AbstractTwoArgClause.h"

class FollowsClause: public AbstractTwoArgClause {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
