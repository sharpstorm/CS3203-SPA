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
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  PQLSynonymName synonym;
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

bool FollowsClause::usesSynonym(string varName) {
  return (left.isSynonym() && left.getSynonymName() == varName)
      || (right.isSynonym() && right.getSynonymName() == varName);
}
