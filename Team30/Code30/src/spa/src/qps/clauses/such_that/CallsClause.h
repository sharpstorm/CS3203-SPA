#pragma once

#include <memory>

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"

using std::shared_ptr;

class CallsClause: public AbstractTwoArgClause {
 public:
  CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
