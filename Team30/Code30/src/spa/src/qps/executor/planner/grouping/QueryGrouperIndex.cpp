#include "QueryGrouperIndex.h"

void QueryGrouperIndex::insertUsage(const PQLSynonymName &name, int value) {
  if (synonymMap.find(name) != synonymMap.end()) {
    synonymMap[name].insert(value);
  } else {
    synonymMap[name] = PlanNodes{value};
  }
}

PlanNodes *QueryGrouperIndex::getUsages(const PQLSynonymName &key) {
  if (synonymMap.find(key) == synonymMap.end()) {
    return nullptr;
  }
  return &synonymMap.at(key);
}

void QueryGrouperIndex::insertSelection(const PQLSynonymName &name) {
  selectSynonyms.insert(name);
}

void QueryGrouperIndex::insertConstraint(const PQLSynonymName &name) {
  constrainSynonyms.insert(name);
}

bool QueryGrouperIndex::selectSynonym(const PQLSynonymName &name) {
  if (selectSynonyms.find(name) == selectSynonyms.end()) {
    return false;
  }

  selectSynonyms.erase(name);
  return true;
}

void QueryGrouperIndex::linkConstraint(const PQLSynonymName &name) {
  constrainSynonyms.erase(name);
}

const PQLSynonymNameSet* QueryGrouperIndex::getHangingSelects() const {
  return &selectSynonyms;
}

const PQLSynonymNameSet *QueryGrouperIndex::getHangingConstraints() const {
  return &constrainSynonyms;
}
