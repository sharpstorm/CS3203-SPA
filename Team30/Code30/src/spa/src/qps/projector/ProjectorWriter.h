#pragma once

#include <vector>

#include "qps/common/projector_table/ProjectableGroup.h"
#include "ProjectorInstruction.h"

using std::vector;

class ProjectorWriter {
  typedef vector<ProjectorResultRow> RowIndexes;
  typedef int ProjectedRow;

 private:
  static const int BUFFER_SIZE = 1024;

  const ProjectableGroupPtrList *groupResults;

  void populateIndexes(RowIndexes *indexes,
                       const ProjectedRow outputRow) const;
  void projectForRow(const ProjectorIndex &index,
                     const RowIndexes *row,
                     ProjectedValue *outputCache) const;
  int getFinalRowCount() const;

 public:
  explicit ProjectorWriter(const ProjectableGroupPtrList *list);

  ProjectorIndex buildProjectionIndex(const AttributedSynonymList *synList,
                                      const PkbQueryHandler *pkbHandler) const;
  void projectTo(QPSOutputList *output, const ProjectorIndex &index) const;
};
