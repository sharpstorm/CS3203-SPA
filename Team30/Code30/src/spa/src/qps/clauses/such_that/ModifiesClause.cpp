#include <utility>

#include "ModifiesClause.h"

ModifiesClause::ModifiesClause(ClauseArgumentPtr leftArg,
                               ClauseArgumentPtr rightArg):
    AbstractTwoArgClause(std::move(leftArg), std::move(rightArg)) {
}

PQLQueryResult* ModifiesClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
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

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
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

QueryResult<int, string> ModifiesClause::evaluateLeftStatement(
    PkbQueryHandler* pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  StmtRef leftStatement = left->toStmtRef();
  QueryResult<int, string> queryResult =
      pkbQueryHandler->queryModifies(leftStatement, rightEntity);

  return queryResult;
}

QueryResult<string, string> ModifiesClause::evaluateLeftEntity(
    PkbQueryHandler* pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  EntityRef leftEntity = left->toEntityRef();
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryModifies(leftEntity, rightEntity);

  return queryResult;
}
