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

unordered_set<PQLSynonymName>* QueryGrouperIndex::getSelectSynonyms() {
  return &selectSynonyms;
}

bool QueryGrouperIndex::hasSelectables() const {
  return selectSynonyms.size() > 0;
}

bool QueryGrouperIndex::isConstrained(const PQLSynonymName &name) const {
  return constrainSynonyms.find(name) != constrainSynonyms.end();
}
