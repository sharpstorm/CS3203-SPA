#include "SynonymProxyBuilder.h"

void SynonymProxyBuilder::joinSynonyms(const PQLSynonymName &syn1,
                                       const PQLSynonymName &syn2) {
  ufds.mergeSets(syn1, syn2);
}

void SynonymProxyBuilder::SynonymProxyBuilder::build() {
  ProxyMap* proxyMap = variableTable->getProxyMap();
  for (auto i = proxyMap->begin(); i != proxyMap->end(); i++) {
    PQLQuerySynonym* newPtr = ufds.getSetValue(i->first);
    i->second.updatePointer(newPtr);
  }
}

bool SynonymProxyBuilder::resolveOverrideMerging(OverrideTable *overrideTable) {
  ProxyMap* proxyMap = variableTable->getProxyMap();
  for (auto i = proxyMap->begin(); i != proxyMap->end(); i++) {
    PQLQuerySynonym* mappedSynonym = ufds.getSetValue(i->first);
    PQLSynonymName newName = mappedSynonym->getName();
    if (i->first == newName) {
      continue;
    }

    auto oldTransformer = overrideTable->find(i->first);
    if (oldTransformer == overrideTable->end()) {
      continue;
    }

    auto newTransformer = overrideTable->find(newName);
    if (newTransformer == overrideTable->end()) {
      overrideTable->emplace(newName, oldTransformer->second);
      continue;
    }
    return false;
  }

  return true;
}

VariableTable* SynonymProxyBuilder::getTable() {
  return variableTable;
}
