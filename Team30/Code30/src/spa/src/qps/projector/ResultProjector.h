#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/PQLQueryResult.h"

using std::string, std::vector;

class ResultProjector {
 public:
  UniqueVectorPtr<string> project(PQLQueryResult* queryResult);

 private:
  UniqueVectorPtr<string> projectEntities(ENTITY_MAP entityMap);
  UniqueVectorPtr<string> projectStatements(STATEMENT_MAP statementMap);
};
