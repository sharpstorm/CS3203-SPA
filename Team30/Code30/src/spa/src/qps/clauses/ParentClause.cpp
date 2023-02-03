#include "ParentClause.h"

ParentClause::ParentClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

QueryResult* ParentClause::evaluateOn() {
  return nullptr;
}

bool ParentClause::validateArgTypes(VariableTable *variables) {
  return true;
}
