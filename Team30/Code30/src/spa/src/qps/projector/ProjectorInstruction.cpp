#include "ProjectorInstruction.h"

ProjectorInstruction::ProjectorInstruction(
    const ResultGroupId groupId,
    const ResultTableCol col,
    const ResultItemProjector &projector) :
    groupId(groupId), tableCol(col), projector(projector) {}

ResultGroupId ProjectorInstruction::getGroupId() const {
  return groupId;
}

ResultTableCol ProjectorInstruction::getTableCol() const {
  return tableCol;
}

ProjectedValue ProjectorInstruction::project(const QueryResultItem *item)
const {
  return projector.project(item);
}
