#include <memory>
#include <utility>
#include <vector>
#include <unordered_set>

#include "ParentTClause.h"

using std::pair, std::vector, std::shared_ptr, std::move;

ParentTClause::ParentTClause(ClauseArgumentPtr leftArg,
                             ClauseArgumentPtr rightArg):
  AbstractTwoArgClause(std::move(leftArg), std::move(rightArg)) {
}

PQLQueryResult* ParentTClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  if (isSameSynonym()) {
    return Clause::toQueryResult(left->getName(), unordered_set<int>{});
  }

  StmtRef leftStatement = left->toStmtRef();
  StmtRef rightStatement = right->toStmtRef();
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryParentStar(leftStatement, rightStatement);
  return Clause::toQueryResult(left.get(), right.get(), queryResult);
}

bool ParentTClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement);
  bool isRightValid = right->synonymSatisfies(ClauseArgument::isStatement);

  return isLeftValid && isRightValid;
}
