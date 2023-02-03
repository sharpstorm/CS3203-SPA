#include "FollowsClause.h"

FollowsClause::FollowsClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

IntermediateResultSet* FollowsClause::evaluateOn() {
  return nullptr;
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  return true;
}
