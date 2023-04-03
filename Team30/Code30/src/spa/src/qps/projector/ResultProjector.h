#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/projector_table/ProjectorResultTable.h"
#include "qps/common/AttributedSynonym.h"

using std::string, std::vector;

static const char STATIC_TRUE[] = "TRUE";
static const char STATIC_FALSE[] = "FALSE";

class ResultProjector {
 private:
  PkbQueryHandler* pkbQueryHandler;

 public:
  explicit ResultProjector(PkbQueryHandler* handler);
  void project(const ProjectorResultTable* queryResult,
               const AttributedSynonymList* resultVariables,
               QPSOutputList *output);
};
