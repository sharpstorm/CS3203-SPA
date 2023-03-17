#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ResultGroup.h"
#include "common/SetUtils.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/constraint/Constraint.h"

using std::unordered_map, std::unordered_set, std::unique_ptr;

class SynonymResultTable {
  vector<ResultGroupPtr> groupResults;
  bool isBooleanResult;
  bool booleanResult;
  OverrideTablePtr overrideTable;

 public:
  SynonymResultTable(bool isBooleanResult,
                     bool booleanResult);
  ~SynonymResultTable() = default;
  void addResultGroup(ResultGroupPtr rg);
  bool getIsBooleanResult();
  bool getBooleanResult();
  int getResultGroupCount();
  void setOverrideTable(OverrideTablePtr overrideTab);
  OverrideTable* getOverrideTable();
  ResultGroup* getResultGroup(int idx);
  bool operator ==(const SynonymResultTable &srt) const;
};
