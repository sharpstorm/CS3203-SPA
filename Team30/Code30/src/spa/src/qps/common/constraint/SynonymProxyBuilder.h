#pragma once

#include <memory>

#include "SynonymUFDS.h"
#include "OverrideTable.h"

using std::unique_ptr;

class SynonymProxyBuilder {
 private:
  ProxyMap* proxyMap;
  SynonymUFDS ufds;

 public:
  explicit SynonymProxyBuilder(ProxyMap *proxyMap);
  bool joinSynonyms(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2);
  void build();
  bool resolveOverrideMerging(OverrideTable* overrideTable);
};

typedef unique_ptr<SynonymProxyBuilder> SynonymProxyBuilderPtr;
