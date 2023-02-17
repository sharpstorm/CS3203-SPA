#include <memory>
#include <utility>
#include <vector>

#include "ParentTClause.h"
#include "qps/common/adapters/StatementResultBuilder.h"

using std::pair, std::vector, std::shared_ptr, std::move;

ParentTClause::ParentTClause(ClauseArgumentPtr leftArg,
                             ClauseArgumentPtr rightArg):
  left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* ParentTClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
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

bool ParentTClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}
