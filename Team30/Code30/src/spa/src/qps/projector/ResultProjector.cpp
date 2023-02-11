#include <vector>

#include "ResultProjector.h"

using std::to_string, std::vector, std::make_unique;

UniqueVectorPtr<string> ResultProjector::project(PQLQueryResult* queryResult) {
  if (queryResult->getError() != "") {
    vector<string>* result = new vector<string>();
    result->push_back(queryResult->getError());
    return UniqueVectorPtr<string>(result);
  }

  if (queryResult->isEntityMapEmpty()) {
    STATEMENT_MAP statementMap = queryResult->getStatementMap();
    return projectStatements(statementMap);
  } else if (queryResult->isStatementMapEmpty()) {
    ENTITY_MAP entityMap = queryResult->getEntityMap();
    return projectEntities(entityMap);
  }

  return UniqueVectorPtr<string>(new vector<string>());
}

UniqueVectorPtr<string> ResultProjector::projectStatements(
    STATEMENT_MAP statementMap) {
  vector<string>* result = new vector<string>();
  for (auto it= statementMap.begin(); it != statementMap.end(); ++it) {
    StatementResult statementResult = it->second;
    if (!statementResult.linePairs.empty()) {
      for (auto stmt : statementResult.linePairs) {
        result->push_back(to_string(statementResult.isLeftArg ?
                                    stmt.first : stmt.second));
      }
    }

    for (auto stmt : statementResult.lines) {
      result->push_back(to_string(stmt));
    }
  }

  return UniqueVectorPtr<string>(result);
}

UniqueVectorPtr<string> ResultProjector::projectEntities(ENTITY_MAP entityMap) {
  vector<string>* result = new vector<string>();
  for (auto it= entityMap.begin(); it != entityMap.end(); ++it)  {
    EntityResult entityResult = it->second;
    if (!entityResult.entityPairs.empty()) {
      for (auto &entity : entityResult.entityPairs) {
        result->push_back(entityResult.isLeftArg ?
                          to_string(entity.first) : entity.second);
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

  return UniqueVectorPtr<string>(result);
}
