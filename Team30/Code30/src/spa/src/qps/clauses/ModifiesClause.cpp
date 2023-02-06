#include <memory>

#include "ModifiesClause.h"

using std::shared_ptr;

ModifiesClause::ModifiesClause(ClauseArgument leftArg, ClauseArgument rightArg):
    left(leftArg), right(rightArg) {
}

PQLQueryResult* ModifiesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
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
