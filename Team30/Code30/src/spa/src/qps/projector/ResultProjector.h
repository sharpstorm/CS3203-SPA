#pragma once

#include "common/UtilityTypes.h"

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/common/synonym/PQLQuerySynonym.h"
#include "qps/common/projector_table/ProjectableTable.h"
#include "qps/common/synonym/AttributedSynonym.h"
#include "qps/common/QPSTypes.h"
#include "IProjector.h"

static const char STATIC_TRUE[] = "TRUE";
static const char STATIC_FALSE[] = "FALSE";

class ResultProjector : public IProjector {
 private:
  const PkbQueryHandler *pkbQueryHandler;

 public:
  explicit ResultProjector(const PkbQueryHandler *handler);
  void project(const ProjectableTable *queryResult,
               const AttributedSynonymList *resultVariables,
               QPSOutputList *output) override;
};
