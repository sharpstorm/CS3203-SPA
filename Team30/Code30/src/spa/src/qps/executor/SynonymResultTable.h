#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "ResultGroup.h"
#include "common/SetUtils.h"
#include "qps/common/PQLQueryResult.h"


using std::unordered_map, std::unordered_set, std::unique_ptr;

class SynonymResultTable {
  vector<unique_ptr<ResultGroup>> groupResults;
  bool isBooleanResult;
  bool booleanResult;
//  IntersectSet<int> getUniqueRows(PQLQueryResult* result,
//                                  vector<PQLSynonymName>* syns);

 public:
  SynonymResultTable(bool isBooleanResult, bool booleanResult);
  ~SynonymResultTable() = default;
//  void extractResults(PQLQueryResult* result, vector<PQLSynonymName> syns);
  void addResultGroup(unique_ptr<ResultGroup> rg);
  bool getIsBooleanResult();
  bool getBooleanResult();
  int getResultGroupCount();
  ResultGroup* getResultGroup(int idx);
  bool operator ==(const SynonymResultTable &srt) const;
};
