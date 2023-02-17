#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "FollowsClause.h"
#include "qps/common/adapters/StatementResultBuilder.h"

using std::pair, std::unordered_set, std::vector, std::move;

FollowsClause::FollowsClause(ClauseArgumentPtr leftArg,
                             ClauseArgumentPtr rightArg):
    left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* FollowsClause::evaluateOn(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = left->toStmtRef();
  StmtRef rightStatement = right->toStmtRef();
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryFollows(leftStatement, rightStatement);

  return Clause::stmtQueryToQueryResult(left.get(), right.get(), queryResult);
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement);
  bool isRightValid = right->synonymSatisfies(ClauseArgument::isStatement);

  return isLeftValid && isRightValid;
}

bool FollowsClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}
