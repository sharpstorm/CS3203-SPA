#include "SynonymHolder.h"

SynonymHolder::SynonymHolder() {
}

SynonymHolder::SynonymHolder(vector<PQLQuerySynonym> syns) {
  for (auto syn : syns) {
    addSyn(syn.getName(), syn.getType());
  }
}

void SynonymHolder::addSyn(PQLSynonymName name, PQLSynonymType type) {
  rawSyns.push_back(make_unique<PQLQuerySynonym>(type, name));
  rawPtrLayer.push_back(make_unique<PQLQuerySynonym*>(rawSyns.back().get()));
  PQLQuerySynonymProxy proxy(rawPtrLayer.back().get());
  proxies.emplace(name, proxy);
}

PQLQuerySynonymProxy SynonymHolder::getProxy(PQLSynonymName name) {
  return proxies.at(name);
}
