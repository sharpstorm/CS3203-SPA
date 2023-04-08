#pragma once

#include "qps/common/projector_table/ProjectableGroup.h"
#include "qps/common/QPSTypes.h"

class ProjectableTable {
 private:
  ProjectableGroupPtrList groupResults;
  bool staticResult;

 public:
  explicit ProjectableTable(bool staticResult);
  ~ProjectableTable() = default;

  void addResultGroup(ProjectableGroupPtr rg);
  bool isStaticTrue() const;
  bool hasGroups() const;
  const ProjectableGroupPtrList *getGroups() const;

  bool operator==(const ProjectableTable &srt) const;
};
