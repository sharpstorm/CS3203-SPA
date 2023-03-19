#include <unordered_set>
#include <unordered_map>

#include "VariableTable.h"
#include "PQLQuerySynonymProxy.h"

using std::unordered_set;

VariableTable::VariableTable() {}

VariableTable::VariableTable(unordered_map<string, PQLQuerySynonym> map) {
  for (auto it : map) {
    add(it.first, it.second);
  }
  finalizeTable();
}

int VariableTable::size() {
  return declaredSynonyms.size();
}


PQLQuerySynonymProxy* VariableTable::find(const PQLSynonymName &name) {
  auto item = proxyMap.find(name);
  if (item == proxyMap.end()) {
    return nullptr;
  }
  return &item->second;
}

void VariableTable::add(const PQLSynonymName &name, PQLQuerySynonym syn) {
  declaredSynonyms.push_back(syn);
}

void VariableTable::finalizeTable() {
  for (int i = 0; i < declaredSynonyms.size(); i++) {
    PQLQuerySynonym* syn = &(declaredSynonyms.at(i));
    rawProxy.push_back(syn);
  }

  for (int i = 0; i < declaredSynonyms.size(); i++) {
    PQLQuerySynonym* syn = &(declaredSynonyms.at(i));
    proxyMap.emplace(syn->getName(), PQLQuerySynonymProxy(&rawProxy.at(i)));
  }
}

unordered_set<PQLSynonymName> VariableTable::getReferredSynonyms() {
  unordered_set<PQLSynonymName> result;

  for (auto it = proxyMap.begin(); it != proxyMap.end(); it++) {
    result.insert(it->second->getName());
  }

  return result;
}

unordered_map<string, PQLQuerySynonymProxy>* VariableTable::getProxyMap() {
  return &proxyMap;
}
