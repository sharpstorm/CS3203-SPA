#pragma once

#include <unordered_map>
#include <unordered_set>
#include "qps/common/synonym/PQLTypes.h"

using std::unordered_set, std::unordered_map;

typedef int PlanNode;
typedef unordered_set<PlanNode> PlanNodes;

class QueryGrouperIndex {
 private:
  unordered_map<PQLSynonymName, PlanNodes> synonymMap;
  PQLSynonymNameSet selectSynonyms;
  PQLSynonymNameSet constrainSynonyms;

 public:
  PlanNodes* getUsages(const PQLSynonymName &key);
  void insertUsage(const PQLSynonymName &name, PlanNode value);
  void insertSelection(const PQLSynonymName &name);
  void insertConstraint(const PQLSynonymName &name);
  bool selectSynonym(const PQLSynonymName &name);
  void linkConstraint(const PQLSynonymName &name);
  const PQLSynonymNameSet* getHangingSelects() const;
  const PQLSynonymNameSet *getHangingConstraints() const;
};
