#pragma once

#include <string>
#include "../common/QueryResult.h"

using std::string;

class ResultProjector {
 public:
  string* project(QueryResult* queryResult);
  string* projectEntities(ENTITY_MAP entityMap);
  string* projectStatements(STATEMENT_MAP statementMap);
};
