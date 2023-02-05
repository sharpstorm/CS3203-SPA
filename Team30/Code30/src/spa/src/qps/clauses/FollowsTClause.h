#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class FollowsTClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  FollowsTClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
