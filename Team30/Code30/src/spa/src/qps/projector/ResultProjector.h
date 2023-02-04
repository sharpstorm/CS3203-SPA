#pragma once

#include <string>
#include "../common/QueryResult.h"

using std::string;

class ResultProjector {
 public:
  string* project(QueryResult* queryResult);
  void projectEntities(string* result, ENTITY_MAP entityMap);
  void projectStatements(string* result, STATEMENT_MAP statementMap);
};
