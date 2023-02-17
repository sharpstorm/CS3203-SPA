#include <memory>
#include <utility>

#include "ModifiesClause.h"
//#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::shared_ptr, std::move;

ModifiesClause::ModifiesClause(ClauseArgumentPtr leftArg,
                               ClauseArgumentPtr rightArg):
    left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* ModifiesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  if (left->synonymSatisfies(ClauseArgument::isStatement)) {
    return generateQueryResult(evaluateLeftStatement(pkbQueryHandler));
  } else {
    return generateQueryResult(evaluateLeftEntity(pkbQueryHandler));
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
  EntityRef rightEntity = right->toEntityRef();
  StmtRef leftStatement = left->toStmtRef();
  QueryResult<int, string> queryResult =
      pkbQueryHandler->queryModifies(leftStatement, rightEntity);

  return queryResult;
}

QueryResult<string, string> ModifiesClause::evaluateLeftEntity(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  EntityRef leftEntity = left->toEntityRef();
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryModifies(leftEntity, rightEntity);

  return queryResult;
}

template<typename T>
PQLQueryResult *ModifiesClause::generateQueryResult(
    QueryResult<T, string> queryResult) {
  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
  if (!left->isNamed() && !right->isNamed()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  left->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
    EntityResult result = EntityResultBuilder::buildEntityResult(true, queryResult);
    pqlQueryResult->addToEntityMap(name, result);
  });

  right->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
    EntityResult result = EntityResultBuilder::buildEntityResult(false, queryResult);
    pqlQueryResult->addToEntityMap(name, result);
  });

  return pqlQueryResult;
}
