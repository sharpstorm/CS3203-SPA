#include <string>

#include "UsesClause.h"
#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::string;

UsesClause::UsesClause(ClauseArgument leftArg, ClauseArgument rightArg):
  left(leftArg), right(rightArg) {
}

PQLQueryResult* UsesClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  if (left.isEntRef()) {
    return generateQueryResult(evaluateLeftEntity(pkbQueryHandler));
  } else {
    return generateQueryResult(evaluateLeftStatement(pkbQueryHandler));
  }
}

bool UsesClause::validateArgTypes(VariableTable *variables) {
  if (left.isWildcard()) {
    return false;
  }

  if (left.isSynonym()) {
    PQLQueryVariable leftVar = variables->at(left.getSynonymName());
    if (!leftVar.isType(PQL_VAR_TYPE_ASSIGN)
        && !leftVar.isType(PQL_VAR_TYPE_PRINT)
        && !leftVar.isType(PQL_VAR_TYPE_IF)
        && !leftVar.isType(PQL_VAR_TYPE_WHILE)
        && !leftVar.isType(PQL_VAR_TYPE_PROCEDURE)
        && !leftVar.isType(PQL_VAR_TYPE_CALL)) {
      return false;
    }
  }

  if (right.isSynonym()
      && !variables->at(right.getSynonymName())
          .isType(PQL_VAR_TYPE_VARIABLE)) {
    return false;
  }
  return true;
}

bool UsesClause::usesSynonym(string varName) {
  return (left.isSynonym() && left.getSynonymName() == varName)
      || (right.isSynonym() && right.getSynonymName() == varName);
}

QueryResult<int, string> UsesClause::evaluateLeftStatement(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = ClauseArgumentRef::toEntityRef(right);
  StmtRef leftStatement = ClauseArgumentRef::toStmtRef(left);
  QueryResult<int, string> queryResult =
      pkbQueryHandler->queryUses(leftStatement, rightEntity);

  return queryResult;
}

QueryResult<string, string> UsesClause::evaluateLeftEntity(
    shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  EntityRef rightEntity = ClauseArgumentRef::toEntityRef(right);
  EntityRef leftEntity = ClauseArgumentRef::toEntityRef(left);
  QueryResult<string, string> queryResult =
      pkbQueryHandler->queryUses(leftEntity, rightEntity);

  return queryResult;
}

template<typename T>
PQLQueryResult *UsesClause::generateQueryResult(
    QueryResult<T, string> queryResult) {
  PQLQueryResult* pqlQueryResult = new PQLQueryResult();

  if (!left.isSynonym() && !right.isSynonym()) {
    pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
    return pqlQueryResult;
  }

  PQL_VAR_NAME synonym;
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

