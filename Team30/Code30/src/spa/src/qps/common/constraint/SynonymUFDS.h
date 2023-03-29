#pragma once

#include <vector>
#include <unordered_map>

#include "common/cfg/CFG.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/VariableTable.h"

using std::vector, std::unordered_map;

class SynonymUFDS {
 private:
  vector<int> parent;
  vector<uint16_t> rank;
  unordered_map<PQLSynonymName, int> synNameMap;
  vector<PQLQuerySynonym*> synArr;

 public:
  explicit SynonymUFDS(VariableTable* varTable);

  int findSet(int set);
  bool tryMergeSets(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2);
  bool isSameSet(const PQLSynonymName &syn1,
                 const PQLSynonymName &syn2);
  vector<int> getParents();
  PQLQuerySynonym* getSetValue(const PQLSynonymName &set);

 private:
  int indexOf(const PQLSynonymName &syn);
  bool mergeSyns(const int &setA,
                 const int &setB);
};

