#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ProjectorResultGroup.h"
#include "common/SetUtils.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/constraints/Constraint.h"
#include "qps/projector/ProjectorInstruction.h"

using std::unordered_map, std::unordered_set, std::unique_ptr;

class ProjectorResultTable {
  typedef vector<int> RowIndexes;

  vector<ResultGroupPtr> groupResults;
  bool isBooleanResult;
  bool booleanResult;

  void populateIndexes(RowIndexes *indexes, const int &outputRow) const;
  void projectForRow(const ProjectorIndex &index,
                     const RowIndexes *row,
                     string &outputCache) const;
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

  bool operator ==(const ProjectorResultTable &srt) const;
};
