#include "SynonymProxyBuilder.h"

void SynonymProxyBuilder::joinSynonyms(const PQLSynonymName &syn1,
                                       const PQLSynonymName &syn2) {
  ufds.mergeSets(syn1, syn2);
}

void SynonymProxyBuilder::SynonymProxyBuilder::build() {
  ProxyMap* proxyMap = table->getProxyMap();
  for (auto i = proxyMap->begin(); i != proxyMap->end(); i++) {
    PQLQuerySynonym* newPtr = ufds.getSetValue(i->first);
    i->second.updatePointer(newPtr);
  }
}

VariableTable* SynonymProxyBuilder::getTable() {
  return table;
}
