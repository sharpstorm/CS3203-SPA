#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class FollowsClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  FollowsClause(ClauseArgument left, ClauseArgument right);
  QueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
