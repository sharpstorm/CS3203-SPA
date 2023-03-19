#include "VariableTableProxyBuilder.h"

void VariableTableProxyBuilder::joinSynonyms(const PQLSynonymName &syn1,
                                             const PQLSynonymName &syn2) {
  ufds.mergeSets(syn1, syn2);
}

void VariableTableProxyBuilder::VariableTableProxyBuilder::build() {
  ProxyMap* proxyMap = table->getProxyMap();
  for (auto i = proxyMap->begin(); i != proxyMap->end(); i++) {
    PQLQuerySynonym* newPtr = ufds.getSetValue(i->first);
    i->second.updatePointer(newPtr);
  }
}

VariableTable* VariableTableProxyBuilder::getTable() {
  return table;
}
