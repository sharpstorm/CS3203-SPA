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
    PQLQuerySynonym leftVar = variables->at(left.getSynonymName());
    if (!leftVar.isStatementType()
        && !leftVar.isType(PQL_SYN_TYPE_PROCEDURE)) {
      return false;
    }
  }

  if (right.isSynonym()
      && !variables->at(right.getSynonymName())
          .isType(PQL_SYN_TYPE_VARIABLE)) {
    return false;
  }
  return true;
}

bool ModifiesClause::usesSynonym(string varName) {
  return (left.isSynonym() && left.getSynonymName() == varName)
      || (right.isSynonym() && right.getSynonymName() == varName);
}
