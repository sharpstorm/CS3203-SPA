#include "ModifiesClause.h"

ModifiesClause::ModifiesClause(ClauseArgument leftArg, ClauseArgument rightArg):
    left(leftArg), right(rightArg) {
}

QueryResult* ModifiesClause::evaluateOn() {
  return nullptr;
}

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
  if (left.isWildcard()) {
    return false;
  }

  if (left.isSynonym()) {
    PQLQueryVariable leftVar = variables->at(left.getSynonymName());
    if (!leftVar.isStatementType()
        && !leftVar.isType(PQL_VAR_TYPE_PROCEDURE)) {
      return false;
    }
  }

  if (right.isSynonym()
      && !variables->at(right.getSynonymName())
          .isType(PQL_VAR_TYPE_VARIABLE)) {
    return false;
  }
  return true;
}
