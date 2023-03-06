#pragma once

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"

class CallsClause: public AbstractTwoArgClause {
 public:
  CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
