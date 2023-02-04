#include "ModifiesClause.h"

ModifiesClause::ModifiesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

QueryResult* ModifiesClause::evaluateOn() {
  return nullptr;
}

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
  return true;
}
