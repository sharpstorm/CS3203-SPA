#include "UsesClause.h"

UsesClause::UsesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* UsesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
  return true;
}
