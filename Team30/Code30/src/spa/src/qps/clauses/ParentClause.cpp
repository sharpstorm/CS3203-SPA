#include "ParentClause.h"

ParentClause::ParentClause(ClauseArgument leftArg, ClauseArgument rightArg, ClausePair args):
        SuchThatClause(args), left(leftArg), right(rightArg) {
}

QueryResult* ParentClause::evaluateOn() {
  return nullptr;
}

bool ParentClause::validateArgTypes(VariableTable *variables) {
  return true;
}
