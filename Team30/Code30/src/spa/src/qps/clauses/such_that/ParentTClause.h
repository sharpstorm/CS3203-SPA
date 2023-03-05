#pragma once

#include <string>

#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"

class ParentTClause: public AbstractTwoArgClause {
 public:
  ParentTClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
