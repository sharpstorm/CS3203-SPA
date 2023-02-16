#include <memory>
#include <utility>

#include "ModifiesClause.h"

using std::shared_ptr, std::move;

ModifiesClause::ModifiesClause(ClauseArgumentPtr leftArg,
                               ClauseArgumentPtr rightArg):
    left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* ModifiesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return nullptr;
}

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
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

bool ModifiesClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}
