#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>
#include <iostream>

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

  PQLQueryResult* pqlQueryResult = new PQLQueryResult();

  if (!left->isNamed() && !right->isNamed()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  left->invokeWithName([queryResult, pqlQueryResult](PQLSynonymName name){
    StatementResult result =
        StatementResultBuilder::buildStatementResult(true, queryResult);
    pqlQueryResult->addToStatementMap(name, result);
  });

  right->invokeWithName([queryResult, pqlQueryResult](PQLSynonymName name){
    StatementResult result =
        StatementResultBuilder::buildStatementResult(false, queryResult);
    pqlQueryResult->addToStatementMap(name, result);
  });

  return pqlQueryResult;
}

bool FollowsClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement);
  bool isRightValid = right->synonymSatisfies(ClauseArgument::isStatement);

  return isLeftValid && isRightValid;
}

bool FollowsClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}
