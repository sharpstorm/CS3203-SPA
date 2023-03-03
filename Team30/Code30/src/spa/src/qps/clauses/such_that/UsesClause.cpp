#include <string>
#include <utility>

#include "UsesClause.h"

using std::string;

UsesClause::UsesClause(ClauseArgumentPtr leftArg, ClauseArgumentPtr rightArg):
    AbstractTwoArgClause(std::move(leftArg), std::move(rightArg)) {
}

PQLQueryResult* UsesClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  // Check left is an entity
  if (left->synonymSatisfies(ClauseArgument::isStatement)) {
    return Clause::toQueryResult(
        left.get(), right.get(),
        evaluateLeftStatement(pkbQueryHandler));
  } else {
    return Clause::toQueryResult(
        left.get(), right.get(),
        evaluateLeftEntity(pkbQueryHandler));
  }
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
  if (left->isWildcard()) {
    return false;
  }

  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement)
      || left->synonymSatisfies(
          ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>);

  return isLeftValid && isRightValid;
}

QueryResult<int, string> UsesClause::evaluateLeftStatement(
    PkbQueryHandler* pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  StmtRef leftStatement = left->toStmtRef();
  QueryResult<int, string> queryResult =
      pkbQueryHandler->queryUses(leftStatement, rightEntity);

  return queryResult;
}

QueryResult<string, string> UsesClause::evaluateLeftEntity(
    PkbQueryHandler* pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  EntityRef leftEntity = left->toEntityRef();
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryUses(leftEntity, rightEntity);

  return queryResult;
}
