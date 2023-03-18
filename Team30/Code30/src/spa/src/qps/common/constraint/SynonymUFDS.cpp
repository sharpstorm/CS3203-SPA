#include "SynonymUFDS.h"

SynonymUFDS::SynonymUFDS(VariableTable* varTable):
    parent(varTable->size()),
    rank(varTable->size()),
    synArr(varTable->size()) {
  for (uint16_t i = 0; i < varTable->size(); i++) {
    parent[i] = i;
  }

  int index = 0;
  for (auto i : varTable->getReferredSynonyms()) {
    synArr[index] = i;
    index += 1;
  }
}

vector<int> SynonymUFDS::getParents() {
  return parent;
}

int SynonymUFDS::indexOf(PQLQuerySynonym *syn) {
  for (int i = 0; i < synArr.size(); i++) {
    if (synArr[i] == syn) {
      return i;
    }
  }
  return -1;
}

PQLQuerySynonym* SynonymUFDS::findSet(PQLQuerySynonym *syn) {
  int index = indexOf(syn);
  if (parent[index] == index) {
    return syn;
  }
  PQLQuerySynonym* synParent = synArr[parent[index]];
  return synArr[indexOf(findSet(synParent))];
}

bool SynonymUFDS::isSameSet(PQLQuerySynonym* syn1,
                            PQLQuerySynonym* syn2) {
  return findSet(syn1) == findSet(syn2);
}

void SynonymUFDS::mergeSets(PQLQuerySynonym* syn1,
                            PQLQuerySynonym* syn2) {
  if (isSameSet(syn1, syn2)) {
    return;
  }

  int aParent = indexOf(findSet(syn1));
  int bParent = indexOf(findSet(syn2));

  if (rank[aParent] > rank[bParent]) {
    parent[bParent] = aParent;
  } else {
    parent[aParent] = bParent;
    if (rank[aParent] == rank[bParent]) {
      rank[bParent]++;
    }
  }
}
