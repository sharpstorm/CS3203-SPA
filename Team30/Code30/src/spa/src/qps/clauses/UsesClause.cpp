#include "UsesClause.h"
#include <utility>

using std::move;

UsesClause::UsesClause(ClauseArgumentPtr leftArg, ClauseArgumentPtr rightArg):
  left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* UsesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
  if (left->isWildcard()) {
    return false;
  }

  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement)
      || left->synonymSatisfies(ClauseArgument::isType(
          PQL_SYN_TYPE_PROCEDURE));
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType(PQL_SYN_TYPE_VARIABLE));

  return isLeftValid && isRightValid;
}

bool UsesClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}
