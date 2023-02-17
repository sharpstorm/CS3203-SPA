#include <memory>
#include <utility>
#include <vector>

#include "ParentClause.h"
#include "qps/common/adapters/StatementResultBuilder.h"

using std::pair, std::vector, std::shared_ptr, std::move;

ParentClause::ParentClause(ClauseArgumentPtr leftArg,
                           ClauseArgumentPtr rightArg):
  left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* ParentClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = left->toStmtRef();
  StmtRef rightStatement = right->toStmtRef();
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryParent(leftStatement, rightStatement);

  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
  if (!left->isNamed() && !right->isNamed()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  left->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
    StatementResult result =
        StatementResultBuilder::buildStatementResult(true, queryResult);
    pqlQueryResult->addToStatementMap(name, result);
  });

  right->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
    StatementResult result =
        StatementResultBuilder::buildStatementResult(false, queryResult);
    pqlQueryResult->addToStatementMap(name, result);
  });

  return pqlQueryResult;
}

bool ParentClause::validateArgTypes(VariableTable *variables) {
  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement);
  bool isRightValid = right->synonymSatisfies(ClauseArgument::isStatement);

  return isLeftValid && isRightValid;
}

bool ParentClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}
