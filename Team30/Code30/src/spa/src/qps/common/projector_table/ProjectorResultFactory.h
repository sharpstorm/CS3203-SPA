#pragma once

#include <memory>
#include <vector>

#include "ProjectorResultGroup.h"
#include "ProjectorResultTable.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"

using std::unique_ptr, std::vector;

class ProjectorResultFactory {
  static IntersectSet<ProjectorResultRow> getUniqueRows(PQLQueryResult* result,
                                                        PQLSynonymNameList* syns);
 public:
  static ResultGroupPtr extractResults(PQLQueryResult* result,
                                       PQLSynonymNameList* syns);
};
