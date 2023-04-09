#pragma once

#include <memory>

#include "qps/common/projector_table/ProjectableGroup.h"
#include "qps/common/QPSTypes.h"

using std::unique_ptr;

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

typedef unique_ptr<ProjectableTable> ProjectableTablePtr;
