#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class UsesClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  UsesClause(ClauseArgument left, ClauseArgument right);
  IntermediateResultSet* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
