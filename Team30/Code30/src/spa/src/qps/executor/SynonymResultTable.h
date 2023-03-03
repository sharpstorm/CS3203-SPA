#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "qps/common/PQLQueryResult.h"
#include "ResultGroup.h"

using std::unordered_map, std::unordered_set, std::unique_ptr;

class SynonymResultTable {
  vector<unique_ptr<ResultGroup>> groupResults;
  PQLQuerySynonymList* synonymMapping;
  bool booleanResult;

 public:
  SynonymResultTable(PQLQuerySynonymList* mapping, bool booleanResult);
  explicit SynonymResultTable(PQLQuerySynonymList* mapping);
  explicit SynonymResultTable(bool booleanResult);
  void extractResults(PQLQueryResult* result, vector<PQLSynonymName> syns);
  bool getBooleanResult();
  bool hasTargetSynonyms();
  int getResultGroupCount();
  ResultGroup* getResultGroup(int idx);
};