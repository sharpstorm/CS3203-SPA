#include "SynonymUFDS.h"

SynonymUFDS::SynonymUFDS(VariableTable* varTable):
    parent(varTable->size()),
    rank(varTable->size()),
    synArr(varTable->size()) {
  for (uint16_t i = 0; i < varTable->size(); i++) {
    parent[i] = i;
    rank[i] = 0;
  }

  int index = 0;
  ProxyMap* proxyMap = varTable->getProxyMap();
  for (auto it = proxyMap->begin(); it != proxyMap->end(); it++) {
    synArr[index] = it->second.get();
    synNameMap[it->first] = index;
    index += 1;
  }
}

vector<int> SynonymUFDS::getParents() {
  return parent;
}

int SynonymUFDS::indexOf(const PQLSynonymName &syn) {
  auto it = synNameMap.find(syn);
  if (it == synNameMap.end()) {
    return -1;
  }

  return it->second;
}

PQLQuerySynonym* SynonymUFDS::getSetValue(const PQLSynonymName &syn) {
  int index = indexOf(syn);
  if (index < 0) {
    return nullptr;
  }

  int parent = findSet(index);
  return synArr[parent];
}

int SynonymUFDS::findSet(int node) {
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

bool SynonymUFDS::mergeSyns(const int &setA, const int &setB) {
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
