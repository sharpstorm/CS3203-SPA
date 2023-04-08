#pragma once

#include <string>
#include "common/UtilityTypes.h"

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/projector_table/ProjectableTable.h"
#include "qps/common/AttributedSynonym.h"
#include "qps/common/QPSTypes.h"

using std::string, std::vector;

static const char STATIC_TRUE[] = "TRUE";
static const char STATIC_FALSE[] = "FALSE";

class ResultProjector {
 private:
  const PkbQueryHandler* pkbQueryHandler;

 public:
  explicit ResultProjector(const PkbQueryHandler* handler);
  void project(const ProjectableTable* queryResult,
               const AttributedSynonymList* resultVariables,
               QPSOutputList *output);
};
