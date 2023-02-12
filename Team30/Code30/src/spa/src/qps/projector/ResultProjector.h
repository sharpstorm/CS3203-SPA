#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/PQLQueryResult.h"

using std::string, std::vector;

class ResultProjector {
 public:
//  UniqueVectorPtr<string> project(PQLQueryResult* queryResult);
  UniqueVectorPtr<string> project(PQLQueryResult* queryResult, PQLQueryVariable resultVariable);
//  PQLQueryResult* filterResultTo(PQLQueryResult* queryResult, PQLQueryVariable resultVariable);

 private:
  UniqueVectorPtr<string> projectEntities(EntityResult* entityResult);
  UniqueVectorPtr<string> projectStatements(StatementResult* statementResult);
};
