#include <memory>
#include <utility>
#include <vector>

#include "ParentTClause.h"
#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/StatementResultBuilder.h"

using std::pair, std::vector, std::shared_ptr;

ParentTClause::ParentTClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* ParentTClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  StmtRef leftStatement = ClauseArgumentRef::toStmtRef(left);
  StmtRef rightStatement = ClauseArgumentRef::toStmtRef(right);
  // Waiting for PkbQueryHandler to expose interface
  QueryResult<int, int> queryResult =
      pkbQueryHandler->queryParent(leftStatement, rightStatement);
//    pkbQueryHandler->queryParentT(leftStatement, rightStatement);

  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
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

bool ParentTClause::validateArgTypes(VariableTable *variables) {
  return true;
}
