#pragma once

#include <vector>

#include "ProjectorResultGroup.h"
#include "common/SetUtils.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/constraints/Constraint.h"
#include "qps/projector/ProjectorInstruction.h"

using std::vector;

class ProjectorResultTable {
  typedef vector<ProjectorResultRow> RowIndexes;

  vector<ResultGroupPtr> groupResults;
  bool isBooleanResult;
  bool booleanResult;

  void populateIndexes(RowIndexes *indexes, const int &outputRow) const;
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
