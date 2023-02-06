#include "ResultProjector.h"

using std::to_string;

string *ResultProjector::project(PQLQueryResult* queryResult) {
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
    for (auto stmt : statementResult.linePairs) {
      if (statementResult.isLeftArg) {
        *result += to_string(stmt.first);
      } else {
        *result += to_string(stmt.second);
      }
    }
  }

  return result;
}

string* ResultProjector::projectEntities(ENTITY_MAP entityMap) {
  string* result = new string();
  for (auto it=entityMap.begin(); it != entityMap.end(); ++it)  {
    EntityResult entityResult = it->second;
    for (auto &entity : entityResult.enitityPairs) {
      if (entityResult.isLeftArg) {
        *result += entity.first;
      } else {
        *result += entity.second;
      }
    }
  }

  return result;
}
