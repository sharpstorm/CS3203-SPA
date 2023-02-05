#include "ModifiesClause.h"

ModifiesClause::ModifiesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* ModifiesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
  return true;
}
