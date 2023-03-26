#include "SynonymProxyBuilder.h"

bool SynonymProxyBuilder::joinSynonyms(const PQLSynonymName &syn1,
                                       const PQLSynonymName &syn2) {
  return ufds.tryMergeSets(syn1, syn2);
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

    if (!overrideTable->contains(i->first)) {
      continue;
    }
    OverrideTransformer oldTransformer = overrideTable->get(i->first);

    if (!overrideTable->contains(newName)) {
      overrideTable->insert(newName, oldTransformer);
      continue;
    }
    return false;
  }

  return true;
}

VariableTable* SynonymProxyBuilder::getTable() {
  return variableTable;
}
