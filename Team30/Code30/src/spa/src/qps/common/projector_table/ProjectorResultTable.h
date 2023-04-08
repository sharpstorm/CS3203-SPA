#pragma once

#include <vector>

#include "ProjectorResultGroup.h"
#include "qps/projector/ProjectorInstruction.h"
#include "qps/common/QPSTypes.h"

using std::vector;

class ProjectorResultTable {
  typedef vector<ProjectorResultRow> RowIndexes;
  typedef int ProjectedRow;

  vector<ResultGroupPtr> groupResults;
  bool isBooleanResult;
  bool booleanResult;

  void populateIndexes(RowIndexes *indexes,
                       const ProjectedRow outputRow) const;
  void projectForRow(const ProjectorIndex &index,
                     const RowIndexes *row,
                     ProjectedValue *outputCache) const;
  int getFinalRowCount() const;

 public:
  ProjectorResultTable(bool isBooleanResult, bool booleanResult);
  ~ProjectorResultTable() = default;

  void addResultGroup(ResultGroupPtr rg);
  bool getIsBooleanResult() const;
  bool getBooleanResult() const;
  int getResultGroupCount() const;
  ProjectorIndex buildProjectionIndex(const AttributedSynonymList *synList,
                                      const PkbQueryHandler *pkbHandler) const;
  void projectTo(QPSOutputList *output, const ProjectorIndex &index) const;

  bool operator==(const ProjectorResultTable &srt) const;
};
