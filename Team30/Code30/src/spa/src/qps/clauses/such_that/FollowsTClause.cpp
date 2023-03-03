#include <utility>
#include <vector>
#include <unordered_set>

#include "FollowsTClause.h"

using std::pair, std::vector;

FollowsTClause::FollowsTClause(ClauseArgumentPtr leftArg,
                               ClauseArgumentPtr rightArg):
    AbstractTwoArgClause(std::move(leftArg), std::move(rightArg)) {
}

PQLQueryResult* FollowsTClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  if (isSameSynonym()) {
    return Clause::toQueryResult(left->getName(), unordered_set<int>{});
  }

  StmtRef leftStatement = left->toStmtRef();
  StmtRef rightStatement = right->toStmtRef();
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryFollowsStar(leftStatement, rightStatement);

  return Clause::toQueryResult(left.get(), right.get(), queryResult);
}

bool FollowsTClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement);
  bool isRightValid = right->synonymSatisfies(ClauseArgument::isStatement);

  return isLeftValid && isRightValid;
}
