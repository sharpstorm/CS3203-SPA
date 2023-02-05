#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "FollowsClause.h"
#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/StatementResultBuilder.h"

using std::pair, std::unordered_set, std::vector, std::shared_ptr;

FollowsClause::FollowsClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* FollowsClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = ClauseArgumentRef::toStmtRef(left);
  StmtRef rightStatement = ClauseArgumentRef::toStmtRef(right);
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryFollows(leftStatement, rightStatement);

  PQLQueryResult*  pqlQueryResult = new PQLQueryResult();

  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticTrue(true);
    return pqlQueryResult;
  }

  PQL_VAR_NAME synonym;
  StatementResult result;
  if (left.isSynonym()) {
    synonym = left.getSynonymName();
    result = StatementResultBuilder::buildStatementResult(
        queryResult.firstArgVals, queryResult.pairVals);
    pqlQueryResult->addToStatementMap(synonym, result);
  }

  if (right.isSynonym()) {
    synonym = right.getSynonymName();
    result = StatementResultBuilder::buildStatementResult(
        queryResult.secondArgVals, queryResult.pairVals);
    pqlQueryResult->addToStatementMap(synonym, result);
  }

  return pqlQueryResult;
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  return true;
}
