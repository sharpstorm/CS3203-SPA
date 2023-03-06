#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/resulttable/SynonymResultTable.h"

using std::string, std::vector;

class ResultProjector {
 public:
  UniqueVectorPtr<string> project(SynonymResultTable* queryResult,
                                  PQLQuerySynonymList* resultVariables);
};
