#include "ResultProjector.h"

using std::to_string;

string *ResultProjector::project(QueryResult* queryResult) {
    if (queryResult->getError() != "") {
        return new string(queryResult->getError());
    }

    if (queryResult->getIsStaticTrue()) {
      return new string();
    }

    if (queryResult->isEntityMapEmpty()) {
      STATEMENT_MAP statementMap = queryResult->getStatementMap();
      return projectStatements( statementMap);
    } else if (queryResult->isStatementMapEmpty()) {
      ENTITY_MAP entityMap = queryResult->getEntityMap();
      return projectEntities( entityMap);
    }

    return new string();
}

string* ResultProjector::projectStatements(STATEMENT_MAP statementMap) {
  string* result = new string();
  for (auto it=statementMap.begin(); it != statementMap.end(); ++it) {
    StatementResult statementResult = it->second;
    for (int stmt : statementResult.lines) {
      *result += to_string(stmt);
    }
  }

  return result;
}

string* ResultProjector::projectEntities(ENTITY_MAP entityMap) {
  string* result = new string();
  for (auto it=entityMap.begin(); it != entityMap.end(); ++it)  {
    EntityResult entityResult = it->second;
    for (string &entity : entityResult.entities) {
      *result += entity;
    }
  }

  return result;
}
