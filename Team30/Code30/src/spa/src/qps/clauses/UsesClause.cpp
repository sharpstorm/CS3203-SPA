#include <string>
#include <utility>

#include "UsesClause.h"
// #include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::string;

using std::move;

UsesClause::UsesClause(ClauseArgumentPtr leftArg, ClauseArgumentPtr rightArg):
  left(move(leftArg)), right(move(rightArg)) {
}

PQLQueryResult* UsesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  // Check left is an entity
  if (left->synonymSatisfies(ClauseArgument::isStatement)) {
    return generateQueryResult(evaluateLeftEntity(pkbQueryHandler));
  } else {
    return generateQueryResult(evaluateLeftStatement(pkbQueryHandler));
  }
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
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

bool UsesClause::usesSynonym(string varName) {
  return left->isSynonymCalled(varName) || right->isSynonymCalled(varName);
}

QueryResult<int, string> UsesClause::evaluateLeftStatement(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  StmtRef leftStatement = left->toStmtRef();
  QueryResult<int, string> queryResult =
      pkbQueryHandler->queryUses(leftStatement, rightEntity);

  return queryResult;
}

QueryResult<string, string> UsesClause::evaluateLeftEntity(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = right->toEntityRef();
  EntityRef leftEntity = left->toEntityRef();
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryUses(leftEntity, rightEntity);

  return queryResult;
}

template<typename T>
PQLQueryResult *UsesClause::generateQueryResult(
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

