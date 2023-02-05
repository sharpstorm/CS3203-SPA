#pragma once

#include <string>
#include "qps/common/PQLQueryResult.h"

using std::string;

class ResultProjector {
 public:
  string* project(PQLQueryResult* queryResult);
  string* projectEntities(ENTITY_MAP entityMap);
  string* projectStatements(STATEMENT_MAP statementMap);
};
