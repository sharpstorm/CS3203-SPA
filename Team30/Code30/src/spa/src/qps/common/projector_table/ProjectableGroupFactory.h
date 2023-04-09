#pragma once

#include <memory>
#include <vector>

#include "qps/common/projector_table/ProjectableGroup.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"

using std::unique_ptr, std::vector;

class ProjectableGroupFactory {
  static IntersectSet<ProjectorResultRow> getUniqueRows(
      PQLQueryResult *result, const PQLSynonymNameList *syns);
 public:
  static ProjectableGroupPtr extractResults(PQLQueryResult *result,
                                            const PQLSynonymNameList *syns);
};
