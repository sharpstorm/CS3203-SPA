#include "VariableTable.h"
#include "PQLQuerySynonymProxy.h"

VariableTable::VariableTable() {}

VariableTable::VariableTable(const SynonymMap &map) {
  for (const auto &it : map) {
    add(it.first, it.second);
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

void VariableTable::add(const PQLSynonymName &name,
                        const PQLQuerySynonym &syn) {
  declaredSynonyms.push_back(syn);
}

void VariableTable::finalizeTable() {
  for (int i = 0; i < declaredSynonyms.size(); i++) {
    PQLQuerySynonym *syn = &(declaredSynonyms.at(i));
    rawProxy.push_back(syn);
  }

  for (int i = 0; i < declaredSynonyms.size(); i++) {
    PQLQuerySynonym *syn = &(declaredSynonyms.at(i));
    proxyMap.emplace(syn->getName(), PQLQuerySynonymProxy(&rawProxy.at(i)));
  }
}

PQLSynonymNameSet VariableTable::getReferredSynonyms() const {
  PQLSynonymNameSet result;

  for (auto it = proxyMap.begin(); it != proxyMap.end(); it++) {
    result.insert(it->second->getName());
  }

  return result;
}

ProxyMap *VariableTable::getProxyMap() {
  return &proxyMap;
}
