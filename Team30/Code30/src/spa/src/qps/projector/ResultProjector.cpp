#include "ResultProjector.h"

using std::to_string;

string *ResultProjector::project(QueryResult* queryResult) {
    if (queryResult->getError() != "") {
        return new string(queryResult->getError());
    }

    string result;
    if (queryResult->isEntityMapEmpty()) {
      STATEMENT_MAP statementMap = queryResult->getStatementMap();
      projectStatements(&result, statementMap);
    } else if (queryResult->isStatementMapEmpty()) {
      ENTITY_MAP entityMap = queryResult->getEntityMap();
      projectEntities(&result, entityMap);
    }

    return &result;
}

void ResultProjector::projectStatements(string *result,
                                        STATEMENT_MAP statementMap) {
  for (auto it=statementMap.begin(); it != statementMap.end(); ++it) {
    StatementResult statementResult = it->second;
    for (int stmt : statementResult.lines) {
      *result += to_string(stmt);
    }
  }
}

void ResultProjector::projectEntities(string *result, ENTITY_MAP entityMap) {
  for (auto it=entityMap.begin(); it != entityMap.end(); ++it)  {
    EntityResult entityResult = it->second;
    for (string &entity : entityResult.entities) {
      *result += entity;
    }
  }
}
