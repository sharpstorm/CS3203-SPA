#pragma once

#include <unordered_map>
#include <unordered_set>
#include "qps/common/PQLQueryResult.h"

using std::unordered_map, std::unordered_set;

class SynonymResultTable {
  vector<unordered_map<PQLSynonymName, QueryResultItemSet>> groupResults;
  PQLQuerySynonymList* synonymMapping;
  PQLQuerySynonymList synonyms;
  bool booleanResult;

 public:
  explicit SynonymResultTable(PQLQuerySynonymList* mapping);
  explicit SynonymResultTable(bool booleanResult);
  void extractSynonyms(PQLQueryResult* result);
  bool getBooleanResult();
};