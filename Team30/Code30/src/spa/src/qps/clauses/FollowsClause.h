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
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  StmtRef buildStatementRef(ClauseArgument argument);
  StatementResult buildStatementResult(unordered_set<int> linesSet,
                                       unordered_set<pair<int, int>> pairsSet,
                                       PQL_VAR_NAME varName);
};
