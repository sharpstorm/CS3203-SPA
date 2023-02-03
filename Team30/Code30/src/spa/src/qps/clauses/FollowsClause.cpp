#include "FollowsClause.h"


FollowsClause::FollowsClause(ClauseArgument leftArg, ClauseArgument rightArg):
        left(leftArg), right(rightArg) {}

QueryResult* FollowsClause::evaluateOn() {
  return nullptr;
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  return true;
}
