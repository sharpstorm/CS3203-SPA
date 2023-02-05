#pragma once

#include "ClauseArgument.h"
#include "SuchThatClause.h"
#include "common/Types.h"

class FollowsClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  FollowsClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
  StmtRef buildStatementRef(ClauseArgument argument);
};
