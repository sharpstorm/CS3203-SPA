#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class ParentClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  ParentClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
