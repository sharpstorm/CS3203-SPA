#include "FollowsTClause.h"

FollowsTClause::FollowsTClause(ClauseArgument leftArg, ClauseArgument rightArg,
                               ClausePair args):
        SuchThatClause(args), left(leftArg), right(rightArg) {
}

QueryResult* FollowsTClause::evaluateOn() {
  return nullptr;
}

bool FollowsTClause::validateArgTypes(VariableTable *variables) {
  return true;
}
