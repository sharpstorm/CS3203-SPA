#include "PQLQueryResult.h"

bool PQLQueryResult::isEntityMapEmpty() {
  return entityMap.empty();
}

bool PQLQueryResult::isStatementMapEmpty() {
  return statementMap.empty();
}

StatementResult* PQLQueryResult::getFromStatementMap(PQL_VAR_NAME var) {
  if (statementMap.find(var) == statementMap.end()) {
    return nullptr;
  }

  return &statementMap.find(var)->second;
}

STATEMENT_MAP PQLQueryResult::getStatementMap() {
  return statementMap;
}

ENTITY_MAP PQLQueryResult::getEntityMap() {
  return entityMap;
}

void PQLQueryResult::addToStatementMap(PQL_VAR_NAME name,
                                       StatementResult result) {
  statementMap.insert({name, result});
}

void PQLQueryResult::addToEntityMap(PQL_VAR_NAME name, EntityResult result) {
  entityMap.insert({name, result});
}

EntityResult* PQLQueryResult::getFromEntityMap(PQL_VAR_NAME var) {
  if (entityMap.find(var) == entityMap.end()) {
    return nullptr;
  }
  return &entityMap.find(var)->second;
}

string PQLQueryResult::getError() {
    return error;
}

void PQLQueryResult::setError(string errorMessage) {
  error = errorMessage;
}

bool PQLQueryResult::getIsStaticTrue() {
  return isStaticTrue;
}

void PQLQueryResult::setIsStaticTrue(bool staticRes) {
  isStaticTrue = staticRes;
}

PQLQueryResult* PQLQueryResult::resultFromVariable(PQLQueryVariable queryVar) {
  PQLQueryResult* queryResult = new PQLQueryResult();
  PQL_VAR_NAME var = queryVar.name;
  switch (queryVar.type) {
    case PQL_VAR_TYPE_STMT:
    case PQL_VAR_TYPE_READ:
    case PQL_VAR_TYPE_PRINT:
    case PQL_VAR_TYPE_CALL:
    case PQL_VAR_TYPE_WHILE:
    case PQL_VAR_TYPE_IF:
    case PQL_VAR_TYPE_ASSIGN:
      if (getFromStatementMap(var) != nullptr) {
        queryResult->addToStatementMap(var, *getFromStatementMap(var));
      }
      break;
    case PQL_VAR_TYPE_VARIABLE:
    case PQL_VAR_TYPE_CONSTANT:
    case PQL_VAR_TYPE_PROCEDURE:
      if (getFromEntityMap(var) != nullptr) {
        queryResult->addToEntityMap(var, *getFromEntityMap(var));
      }
      break;
  }
  queryResult->setError(getError());

  return queryResult;
}
