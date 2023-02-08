#include <utility>
#include <vector>

#include <memory>

#include "FollowsTClause.h"
#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/StatementResultBuilder.h"

using std::pair, std::vector, std::shared_ptr;

FollowsTClause::FollowsTClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* FollowsTClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = ClauseArgumentRef::toStmtRef(left);
  StmtRef rightStatement = ClauseArgumentRef::toStmtRef(right);
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryFollowsStar(leftStatement, rightStatement);

  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
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

bool FollowsTClause::validateArgTypes(VariableTable *variables) {
  if (left.isSynonym()
      && !variables->at(left.getSynonymName()).isStatementType()) {
    return false;
  }
  if (right.isSynonym()
      && !variables->at(right.getSynonymName()).isStatementType()) {
    return false;
  }
  return true;
}

bool FollowsTClause::usesSynonym(string varName) {
  return (left.isSynonym() && left.getSynonymName() == varName)
      || (right.isSynonym() && right.getSynonymName() == varName);
}