#include "VariableTable.h"
#include "PQLQuerySynonymProxy.h"

VariableTable::VariableTable() {}

VariableTable::VariableTable(const SynonymMap &map) {
  for (const auto &it : map) {
    add(it.second);
  }
  finalizeTable();
}

int VariableTable::size() const {
  return declaredSynonyms.size();
}

PQLQuerySynonymProxy *const VariableTable::find(const PQLSynonymName &name) {
  auto item = proxyMap.find(name);
  if (item == proxyMap.end()) {
    return nullptr;
  }
  return &item->second;
}

void VariableTable::add(const PQLQuerySynonym &syn) {
  declaredSynonyms.push_back(syn);
}

void VariableTable::finalizeTable() {
  // First, Finalize Raw Proxy so it can't jump in heap
  for (size_t i = 0; i < declaredSynonyms.size(); i++) {
    PQLQuerySynonym *syn = &(declaredSynonyms.at(i));
    rawProxy.push_back(syn);
  }

  // Then, Finalize Proxy using the finalized raw map
  for (size_t i = 0; i < declaredSynonyms.size(); i++) {
    const PQLSynonymName synName = declaredSynonyms.at(i).getName();
    proxyMap.emplace(synName, PQLQuerySynonymProxy(&rawProxy.at(i)));
  }
}

ProxyMap *VariableTable::getProxyMap() {
  return &proxyMap;
}
