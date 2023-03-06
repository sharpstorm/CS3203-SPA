#include <memory>
#include <utility>
#include <string>

#include "CallsClause.h"

using std::move, std::string;

CallsClause::CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
    : AbstractTwoArgClause(move(left), move(right)) {
}

PQLQueryResult *CallsClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  EntityRef leftEntity = left->toEntityRef();
  EntityRef rightEntity = right->toEntityRef();
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryCalls(leftEntity, rightEntity);
  return Clause::toQueryResult(left.get(), right.get(), queryResult);
}

bool CallsClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  return isLeftValid && isRightValid;
}
