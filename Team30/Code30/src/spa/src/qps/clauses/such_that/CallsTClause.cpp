#include <memory>
#include <utility>

#include "CallsTClause.h"

using std::move;

CallsTClause::CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
    : AbstractTwoArgClause(move(left), move(right)) {
}

PQLQueryResult *CallsTClause::evaluateOn
    (shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  return new PQLQueryResult();
}

bool CallsTClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  return isLeftValid && isRightValid;
}
