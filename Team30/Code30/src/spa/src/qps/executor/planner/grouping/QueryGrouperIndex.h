#pragma once

#include <unordered_map>
#include <unordered_set>
#include "qps/common/PQLTypes.h"

using std::unordered_set, std::unordered_map;

typedef int PlanNode;
typedef unordered_set<PlanNode> PlanNodes;

class QueryGrouperIndex {
 private:
  unordered_map<PQLSynonymName, PlanNodes> synonymMap;
  unordered_set<PQLSynonymName> selectSynonyms;

 public:
  PlanNodes* getUsages(const PQLSynonymName &key);
  void insertUsage(const PQLSynonymName &name, PlanNode value);
  void insertSelection(const PQLSynonymName &name);
  bool selectSynonym(const PQLSynonymName &name);
  unordered_set<PQLSynonymName>* getSelectSynonyms();
  bool hasSelectables();
};
