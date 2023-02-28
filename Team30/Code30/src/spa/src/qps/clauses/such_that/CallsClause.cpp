#include <memory>
#include <utility>

#include "CallsClause.h"

using std::move;

CallsClause::CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
    : AbstractTwoArgClause(move(left), move(right)) {
}

PQLQueryResult *CallsClause::evaluateOn
    (shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef leftEntity = left->toEntityRef();
  EntityRef rightEntity = right->toEntityRef();
  // TODO PKB Query
  QueryResult<string, string> queryResult;
  return Clause::toQueryResult(left.get(), right.get(), queryResult);
}

bool CallsClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  return isLeftValid && isRightValid;
}
