#include "FollowsClause.h"


FollowsClause::FollowsClause(ClauseArgument leftArg, ClauseArgument rightArg, pair<ClauseArgument,ClauseArgument> args):
        SuchThatClause(args), left(leftArg), right(rightArg) {}

QueryResult* FollowsClause::evaluateOn() {
  return nullptr;
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  return true;
}
