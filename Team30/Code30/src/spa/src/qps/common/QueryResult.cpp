#include "QueryResult.h"

bool QueryResult::isEntityMapEmpty() {
  return entityMap.empty();
}

bool QueryResult::isStatementMapEmpty() {
  return statementMap.empty();
}

StatementResult* QueryResult::getFromStatementMap(PQL_VAR_NAME var) {
  if (statementMap.find(var) == statementMap.end()) {
    return nullptr;
  }

  return &statementMap.find(var)->second;
}

unordered_map<PQL_VAR_NAME, StatementResult> QueryResult::getStatementMap() {
  return statementMap;
}

unordered_map<PQL_VAR_NAME, EntityResult> QueryResult::getEntityMap() {
  return entityMap;
}

void QueryResult::addToStatementMap(PQL_VAR_NAME name, StatementResult result) {
  statementMap.insert({name, result});
}

void QueryResult::addToEntityMap(PQL_VAR_NAME name, EntityResult result) {
  entityMap.insert({name, result});
}

EntityResult* QueryResult::getFromEntityMap(PQL_VAR_NAME var) {
  if (entityMap.find(var) == entityMap.end()) {
    return nullptr;
  }
  return &entityMap.find(var)->second;
}

string QueryResult::getError() {
    return error;
}

void QueryResult::setError(string errorMessage) {
  error = errorMessage;
}

QueryResult* QueryResult::resultFromVariable(PQLQueryVariable queryVar) {
  QueryResult* queryResult = new QueryResult();
  PQL_VAR_NAME var = queryVar.name;
  switch(queryVar.type) {
    case PQL_VAR_TYPE_STMT:
      // Fallthrough
    case PQL_VAR_TYPE_READ:
      // Fallthrough
    case PQL_VAR_TYPE_PRINT:
      // Fallthrough
    case PQL_VAR_TYPE_CALL:
      // Fallthrough
    case PQL_VAR_TYPE_WHILE:
      // Fallthrough
    case PQL_VAR_TYPE_IF:
      // Fallthrough
    case PQL_VAR_TYPE_ASSIGN:
      if (getFromStatementMap(var) != nullptr) {
        queryResult->addToStatementMap(var, *getFromStatementMap(var));
      }
      break;
    case PQL_VAR_TYPE_VARIABLE:
      // Fallthrough
    case PQL_VAR_TYPE_CONSTANT:
      // Fallthrough
    case PQL_VAR_TYPE_PROCEDURE:
      if (getFromEntityMap(var) != nullptr) {
        queryResult->addToEntityMap(var, *getFromEntityMap(var));
      }
      break;
  }
  queryResult->setError(getError());

  return queryResult;
}