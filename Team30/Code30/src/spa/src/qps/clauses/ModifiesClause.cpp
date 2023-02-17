#include <memory>
#include <utility>

#include "ModifiesClause.h"
#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::shared_ptr, std::move;

ModifiesClause::ModifiesClause(ClauseArgumentPtr leftArg,
                               ClauseArgumentPtr rightArg):
    left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* ModifiesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  if (left.isEntRef()) {
    return generateQueryResult(evaluateLeftEntity(pkbQueryHandler));
  } else {
    return generateQueryResult(evaluateLeftStatement(pkbQueryHandler));
  }
}

bool ModifiesClause::validateArgTypes(VariableTable *variables) {
  if (left->isWildcard()) {
    return false;
  }

  bool isLeftValid = left->synonymSatisfies(ClauseArgument::isStatement)
      || left->synonymSatisfies(
          ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>);
  bool isRightValid = right->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>);

  return isLeftValid && isRightValid;
}

bool ModifiesClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}

QueryResult<int, string> ModifiesClause::evaluateLeftStatement(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = ClauseArgumentRef::toEntityRef(right);
  StmtRef leftStatement = ClauseArgumentRef::toStmtRef(left);
  QueryResult<int, string> queryResult =
      pkbQueryHandler->queryModifies(leftStatement, rightEntity);

  return queryResult;
}

QueryResult<string, string> ModifiesClause::evaluateLeftEntity(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = ClauseArgumentRef::toEntityRef(right);
  EntityRef leftEntity = ClauseArgumentRef::toEntityRef(left);
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryModifies(leftEntity, rightEntity);

  return queryResult;
}

template<typename T>
PQLQueryResult *ModifiesClause::generateQueryResult(
    QueryResult<T, string> queryResult) {
  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  PQLSynonymName synonym;
  EntityResult result;

  if (left.isSynonym()) {
    synonym = left.getSynonymName();
    result = EntityResultBuilder::buildEntityResult(true, queryResult);

    pqlQueryResult->addToEntityMap(synonym, result);
  }

  if (right.isSynonym()) {
    synonym = right.getSynonymName();
    result = EntityResultBuilder::buildEntityResult(false, queryResult);
    pqlQueryResult->addToEntityMap(synonym, result);
  }

  return pqlQueryResult;
}
