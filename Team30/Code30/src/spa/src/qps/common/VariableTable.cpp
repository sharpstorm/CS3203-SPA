#include <unordered_set>

#include "VariableTable.h"

using std::unordered_set;

VariableTable::VariableTable() {}

VariableTable::VariableTable(unordered_map<string, PQLQuerySynonym> map) {
  for (auto it : map) {
    add(it.first, it.second);
  }
}

PQLQuerySynonymProxy VariableTable::getProxyFor(const PQLSynonymName &name) {
  return &proxyArray[name];
}

int VariableTable::size() {
  return declaredSynonyms.size();
}


PQLQuerySynonym* VariableTable::find(const PQLSynonymName &name) {
  auto item = proxyArray.find(name);
  if (item == proxyArray.end()) {
    return nullptr;
  }
  return item->second;
}

void VariableTable::add(const PQLSynonymName &name, PQLQuerySynonym syn) {
  declaredSynonyms.push_back(syn);
  proxyArray.emplace(name, &(declaredSynonyms.back()));
}

unordered_set<PQLQuerySynonym*> VariableTable::getReferredSynonyms() {
  unordered_set<PQLQuerySynonym*> set;
  for (auto i : proxyArray) {
    set.insert(i.second);
  }
  return set;
}

vector<PQLQuerySynonym> VariableTable::getDeclaredSynonyms() {
  return declaredSynonyms;
}
