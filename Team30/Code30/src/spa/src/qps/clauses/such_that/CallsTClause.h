#pragma once

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"

class CallsTClause: public AbstractTwoArgClause {
 public:
  CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
