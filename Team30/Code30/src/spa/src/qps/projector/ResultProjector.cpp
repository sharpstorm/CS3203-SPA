#include <vector>

#include "ResultProjector.h"

using std::to_string, std::vector;

vector<string> *ResultProjector::project(PQLQueryResult* queryResult) {
  if (queryResult->getError() != "") {
    vector<string>* result = new vector<string>();
    result->push_back(queryResult->getError());
    return result;
  }

  if (queryResult->isEntityMapEmpty()) {
    STATEMENT_MAP statementMap = queryResult->getStatementMap();
    return projectStatements(statementMap);
  } else if (queryResult->isStatementMapEmpty()) {
    ENTITY_MAP entityMap = queryResult->getEntityMap();
    return projectEntities(entityMap);
  }

  return new vector<string>();
}

vector<string>* ResultProjector::projectStatements(STATEMENT_MAP statementMap) {
  vector<string>* result = new vector<string>();
  for (auto it=statementMap.begin(); it != statementMap.end(); ++it) {
    StatementResult statementResult = it->second;
    if (!statementResult.linePairs.empty()) {
      for (auto stmt : statementResult.linePairs) {
        if (statementResult.isLeftArg) {
          result->push_back(to_string(stmt.first));
        } else {
          result->push_back(to_string(stmt.second));
        }
      }
    } else {
      for (auto stmt : statementResult.lines) {
        result->push_back(to_string(stmt));
      }
    }
  }

  return result;
}

vector<string>* ResultProjector::projectEntities(ENTITY_MAP entityMap) {
  vector<string>* result = new vector<string>();
  for (auto it=entityMap.begin(); it != entityMap.end(); ++it)  {
    EntityResult entityResult = it->second;
    if (!entityResult.enitityPairs.empty()) {
      for (auto &entity : entityResult.enitityPairs) {
        if (entityResult.isLeftArg) {
          result->push_back(to_string(entity.first));
        } else {
          result->push_back(entity.second);
        }
      }
    } else {
      for (auto entity : entityResult.lines) {
        result->push_back(to_string(entity));
      }
      for (auto entity : entityResult.entities) {
        result->push_back(entity);
      }
    }
  }

  return result;
}
