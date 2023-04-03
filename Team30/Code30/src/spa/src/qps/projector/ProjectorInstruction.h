#pragma once

#include <vector>

#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "ResultItemProjector.h"

class ProjectorInstruction;
typedef int ResultGroupId;
typedef vector<ProjectorInstruction> ProjectorIndex;

class ProjectorInstruction {
 private:
  ResultGroupId groupId;
  ResultTableCol tableCol;
  ResultItemProjector projector;

 public:
  ProjectorInstruction(const ResultGroupId &groupId,
                       const ResultTableCol &col,
                       const ResultItemProjector &projector);
  ResultGroupId getGroupId() const;
  ResultTableCol getTableCol() const;
  ProjectedValue project(const QueryResultItem *item) const;
};
