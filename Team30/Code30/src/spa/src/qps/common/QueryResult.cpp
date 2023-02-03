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

EntityResult* QueryResult::getFromEntityMap(PQL_VAR_NAME var) {
  if (entityMap.find(var) == entityMap.end()) {
    return nullptr;
  }
  return &entityMap.find(var)->second;
}

string QueryResult::getError() {
    return error;
}
