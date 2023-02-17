#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuerySynonym.h"

using std::string, std::vector;

class ResultProjector {
 public:
  UniqueVectorPtr<string> project(PQLQueryResult* queryResult,
                                  PQLQuerySynonym resultVariable);
};
