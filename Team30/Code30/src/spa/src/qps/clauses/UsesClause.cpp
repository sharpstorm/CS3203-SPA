#include "UsesClause.h"

UsesClause::UsesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

QueryResult* UsesClause::evaluateOn() {
  return nullptr;
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
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
