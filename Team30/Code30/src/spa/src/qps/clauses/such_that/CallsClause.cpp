#include <memory>
#include <utility>

#include "CallsClause.h"

using std::move;

CallsClause::CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
    : AbstractTwoArgClause(move(left), move(right)) {
}

PQLQueryResult *CallsClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  return new PQLQueryResult();
}

bool CallsClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  return isLeftValid && isRightValid;
}
