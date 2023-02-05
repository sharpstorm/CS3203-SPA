#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class ModifiesClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  ModifiesClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
