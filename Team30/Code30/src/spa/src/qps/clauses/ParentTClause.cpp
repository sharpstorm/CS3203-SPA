#include "ParentTClause.h"

ParentTClause::ParentTClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

IntermediateResultSet* ParentTClause::evaluateOn() {
  return nullptr;
}

bool ParentTClause::validateArgTypes(VariableTable *variables) {
  return true;
}
