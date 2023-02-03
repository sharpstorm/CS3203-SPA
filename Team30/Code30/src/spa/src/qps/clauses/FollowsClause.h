#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class FollowsClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  FollowsClause(ClauseArgument left, ClauseArgument right, pair<ClauseArgument, ClauseArgument> args);
  QueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
