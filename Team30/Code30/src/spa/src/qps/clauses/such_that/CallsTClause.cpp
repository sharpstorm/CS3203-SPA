#include <memory>
#include <utility>

#include "CallsTClause.h"

using std::move;

CallsTClause::CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
    : AbstractTwoArgClause(move(left), move(right)) {
}

PQLQueryResult *CallsTClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  EntityRef leftEntity = left->toEntityRef();
  EntityRef rightEntity = right->toEntityRef();
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryCallsStar(leftEntity, rightEntity);
  return Clause::toQueryResult(left.get(), right.get(), queryResult);
}

bool CallsTClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  return isLeftValid && isRightValid;
}
