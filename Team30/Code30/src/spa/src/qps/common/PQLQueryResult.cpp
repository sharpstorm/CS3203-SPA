#include "PQLQueryResult.h"

PQLQueryResult::PQLQueryResult():
    isStaticFalse(false), error("") {}

bool PQLQueryResult::isEntityMapEmpty() {
  bool isEmpty = true;
  for (auto &it : entityMap) {
    isEmpty = isEmpty & it.second.isEmpty();
  }
  return isEmpty;
}

bool PQLQueryResult::isStatementMapEmpty() {
  bool isEmpty = true;
  for (auto &it : statementMap) {
    isEmpty = isEmpty & it.second.isEmpty();
  }
  return isEmpty;
}

bool PQLQueryResult::isStaticResult() {
  return statementMap.empty() && entityMap.empty();
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

bool PQLQueryResult::getIsStaticFalse() {
  return isStaticFalse;
}

void PQLQueryResult::setIsStaticFalse(bool staticRes) {
  isStaticFalse = staticRes;
}

PQLQueryResult* PQLQueryResult::filterResultTo(PQLQueryVariable queryVar) {
  PQLQueryResult* queryResult = new PQLQueryResult();

  if (isStaticFalse) {
    queryResult->setIsStaticFalse(isStaticFalse);
    return queryResult;
  }

  PQL_VAR_NAME var = queryVar.name;
  if (queryVar.isStatementType()) {
    StatementResult* result = getFromStatementMap(var);
    if (result != nullptr) {
      queryResult->addToStatementMap(var, *result);
    }
  } else if (queryVar.isEntityType()) {
    EntityResult* result = getFromEntityMap(var);
    if (result != nullptr) {
      queryResult->addToEntityMap(var, *result);
    }
  }
  queryResult->setError(getError());

  return queryResult;
}
bool PQLQueryResult::operator==(PQLQueryResult pqr) const {
  bool statementMapEqual = statementMap == pqr.getStatementMap();
  bool entityMapEqual = entityMap == pqr.getEntityMap();
  bool errorEqual = error == pqr.getError();
  bool isStaticFalseEqual = isStaticFalse == pqr.getIsStaticFalse();
  return statementMapEqual && entityMapEqual && errorEqual &&
      isStaticFalseEqual;
}
