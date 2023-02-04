#include "ModifiesClause.h"

ModifiesClause::ModifiesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

IntermediateResultSet* ModifiesClause::evaluateOn() {
  return nullptr;
}

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
  return true;
}
