#pragma once

#include <vector>

#include "qps/common/projector_table/ProjectableGroup.h"
#include "qps/projector/ProjectorInstruction.h"
#include "qps/common/QPSTypes.h"

using std::vector;

class ProjectableTable {
  typedef vector<ProjectorResultRow> RowIndexes;
  typedef int ProjectedRow;

  vector<ProjectableGroupPtr> groupResults;
  bool isBooleanResult;
  bool booleanResult;

  void populateIndexes(RowIndexes *indexes,
                       const ProjectedRow outputRow) const;
  void projectForRow(const ProjectorIndex &index,
                     const RowIndexes *row,
                     ProjectedValue *outputCache) const;
  int getFinalRowCount() const;

 public:
  ProjectableTable(bool isBooleanResult, bool booleanResult);
  ~ProjectableTable() = default;

  void addResultGroup(ProjectableGroupPtr rg);
  bool getIsBooleanResult() const;
  bool getBooleanResult() const;
  int getResultGroupCount() const;
  ProjectorIndex buildProjectionIndex(const AttributedSynonymList *synList,
                                      const PkbQueryHandler *pkbHandler) const;
  void projectTo(QPSOutputList *output, const ProjectorIndex &index) const;

  bool operator==(const ProjectableTable &srt) const;
};
