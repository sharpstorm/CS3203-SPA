#include "SynonymUFDS.h"

SynonymUFDS::SynonymUFDS(const ProxyMap* proxyMap):
    parent(proxyMap->size()),
    rank(proxyMap->size()),
    synArr(proxyMap->size()) {
  for (size_t i = 0; i < proxyMap->size(); i++) {
    parent[i] = i;
    rank[i] = 0;
  }

  int index = 0;
  for (auto it = proxyMap->begin(); it != proxyMap->end(); it++) {
    synArr[index] = it->second.get();
    synNameMap[it->first] = index;
    index += 1;
  }
}

vector<UFDSSet> SynonymUFDS::getParents() const {
  return parent;
}

UFDSSet SynonymUFDS::indexOf(const PQLSynonymName &syn) const {
  const auto it = synNameMap.find(syn);
  if (it == synNameMap.end()) {
    return -1;
  }

  return it->second;
}

PQLQuerySynonym* SynonymUFDS::getSynFor(const PQLSynonymName &syn) {
  int index = indexOf(syn);
  if (index < 0) {
    return nullptr;
  }

  UFDSSet parent = findSet(index);
  return synArr[parent];
}

UFDSSet SynonymUFDS::findSet(const UFDSSet node) {
  if (parent[node] == node) {
    return node;
  }

  parent[node] = findSet(parent[node]);
  return parent[node];
}

bool SynonymUFDS::isSameSet(const PQLSynonymName &syn1,
                            const PQLSynonymName &syn2) {
  int setA = indexOf(syn1);
  int setB = indexOf(syn2);
  if (setA < 0 || setB < 0) {
    return false;
  }

  return findSet(setA) == findSet(setB);
}

bool SynonymUFDS::tryMergeSets(const PQLSynonymName &syn1,
                               const PQLSynonymName &syn2) {
  int setA = indexOf(syn1);
  int setB = indexOf(syn2);
  if (setA < 0 || setB < 0) {
    return false;
  }

  if (isSameSet(syn1, syn2)) {
    return true;
  }

  int aParent = findSet(setA);
  int bParent = findSet(setB);

  if (rank[aParent] > rank[bParent]) {
    parent[bParent] = aParent;
  } else {
    parent[aParent] = bParent;
    if (rank[aParent] == rank[bParent]) {
      rank[bParent]++;
    }
  }
  return mergeSyns(aParent, bParent);
}

bool SynonymUFDS::mergeSyns(const UFDSSet setA, const UFDSSet setB) {
  PQLQuerySynonym* synA = synArr[setA];
  PQLQuerySynonym* synB = synArr[setB];

  if (synA->isDistinctSubtypeFrom(synB)) {
    return false;
  }

  if (synA->isSubtypeOf(synB)) {
    synArr[setB] = synA;
  } else {
    synArr[setA] = synB;
  }

  return true;
}
