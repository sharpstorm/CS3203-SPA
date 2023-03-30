#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ProjectorResultGroup.h"
#include "common/SetUtils.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/constraints/Constraint.h"

using std::unordered_map, std::unordered_set, std::unique_ptr;

class ProjectorResultTable {
  vector<ResultGroupPtr> groupResults;
  bool isBooleanResult;
  bool booleanResult;

 public:
  ProjectorResultTable(bool isBooleanResult,
                       bool booleanResult);
  ~ProjectorResultTable() = default;
  void addResultGroup(ResultGroupPtr rg);
  bool getIsBooleanResult();
  bool getBooleanResult();
  int getResultGroupCount();
  ProjectorResultGroup* getResultGroup(int idx);
  bool operator ==(const ProjectorResultTable &srt) const;
};
