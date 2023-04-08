#include "SynonymProxyBuilder.h"

SynonymProxyBuilder::SynonymProxyBuilder(ProxyMap *proxyMap) :
    proxyMap(proxyMap), ufds(SynonymUFDS(proxyMap)) {}

bool SynonymProxyBuilder::joinSynonyms(const PQLSynonymName &syn1,
                                       const PQLSynonymName &syn2) {
  return ufds.tryMergeSets(syn1, syn2);
}

void SynonymProxyBuilder::SynonymProxyBuilder::build() {
  for (auto it = proxyMap->begin(); it != proxyMap->end(); it++) {
    PQLQuerySynonym *newPtr = ufds.getSynFor(it->first);
    it->second.updatePointer(newPtr);
  }
}

bool SynonymProxyBuilder::resolveOverrideMerging(OverrideTable *overrideTable) {
  for (auto i = proxyMap->begin(); i != proxyMap->end(); i++) {
    PQLQuerySynonym *mappedSynonym = ufds.getSynFor(i->first);
    PQLSynonymName newName = mappedSynonym->getName();
    if (i->first == newName) {
      continue;
    }

    // There is no transformer to carry over
    if (!overrideTable->contains(i->first)) {
      continue;
    }
    OverrideTransformer oldTransformer = overrideTable->get(i->first);

    // There is no new transformer to merge into
    if (!overrideTable->contains(newName)) {
      overrideTable->insert(newName, oldTransformer);
      continue;
    }

    if (overrideTable->get(newName) == oldTransformer) {
      continue;
    }

    return false;
  }

  return true;
}
