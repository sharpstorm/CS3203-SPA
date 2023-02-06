#pragma once

#include <string>
#include <vector>

#include "qps/common/PQLQueryResult.h"

using std::string, std::vector;

class ResultProjector {
 public:
  vector<string>* project(PQLQueryResult* queryResult);
  vector<string>* projectEntities(ENTITY_MAP entityMap);
  vector<string>* projectStatements(STATEMENT_MAP statementMap);
};
