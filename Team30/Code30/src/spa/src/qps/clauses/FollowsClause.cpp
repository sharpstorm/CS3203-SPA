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
//  QueryResult<int, int> queryResult =
//      pkbQueryHandler->queryFollows(leftStatement, rightStatement);
  QueryResult<int, int> queryResult;
  unordered_set<pair<int, int>> tempPairs;
  tempPairs.insert({1, 2});
  tempPairs.insert({2, 3});
  tempPairs.insert({3,4});
  tempPairs.insert({4,5});
  queryResult.firstArgVals.insert(1);
  for (int i = 1; i < 5; i++) {
    queryResult.secondArgVals.insert(i + 1);
  }
  queryResult.isEmpty = false;

  PQLQueryResult*  pqlQueryResult = new PQLQueryResult();

  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticTrue(true);
    return pqlQueryResult;
  }

  PQL_VAR_NAME synonym;
  StatementResult result;
  if (left.isSynonym()) {
    synonym = left.getSynonymName();
    result = StatementResultBuilder::buildStatementResult(true,
                                                          queryResult);
    pqlQueryResult->addToStatementMap(synonym, result);
  }

  if (right.isSynonym()) {
    synonym = right.getSynonymName();
    result = StatementResultBuilder::buildStatementResult(false,
                                                          queryResult);
    pqlQueryResult->addToStatementMap(synonym, result);
  }

  return pqlQueryResult;
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  return true;
}
