#include "FollowsTClause.h"

FollowsTClause::FollowsTClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

IntermediateResultSet* FollowsTClause::evaluateOn() {
  return nullptr;
}

bool FollowsTClause::validateArgTypes(VariableTable *variables) {
  return true;
}
