#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/projector_table/ProjectorResultTable.h"
#include "qps/common/AttributedSynonym.h"

using std::string, std::vector;

class ResultProjector {
  PkbQueryHandler* pkbQueryHandler;
 public:
  explicit ResultProjector(PkbQueryHandler* handler);
  void project(ProjectorResultTable* queryResult,
               AttributedSynonymList* resultVariables,
               QPSOutputList *output);
};
