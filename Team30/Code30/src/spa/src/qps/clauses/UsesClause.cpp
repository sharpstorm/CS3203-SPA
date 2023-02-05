#include "UsesClause.h"

UsesClause::UsesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* UsesClause::evaluateOn() {
  return nullptr;
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
  return true;
}
