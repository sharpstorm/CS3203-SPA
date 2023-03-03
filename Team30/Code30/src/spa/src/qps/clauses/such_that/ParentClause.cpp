#include <memory>
#include <utility>
#include <vector>
#include <unordered_set>

#include "ParentClause.h"

using std::pair, std::vector, std::shared_ptr;

ParentClause::ParentClause(ClauseArgumentPtr leftArg,
                           ClauseArgumentPtr rightArg):
    AbstractTwoArgClause(std::move(leftArg), std::move(rightArg)) {
}

PQLQueryResult* ParentClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  if (isSameSynonym()) {
    return Clause::toQueryResult(left->getName(), unordered_set<int>{});
  }

  StmtRef leftStatement = left->toStmtRef();
  StmtRef rightStatement = right->toStmtRef();
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryParent(leftStatement, rightStatement);
  return Clause::toQueryResult(left.get(), right.get(), queryResult);
}

bool ParentClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement);
  bool isRightValid = right->synonymSatisfies(ClauseArgument::isStatement);

  return isLeftValid && isRightValid;
}
