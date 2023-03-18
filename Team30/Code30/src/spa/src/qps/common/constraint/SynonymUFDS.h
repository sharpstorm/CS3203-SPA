#pragma once

#include <vector>

#include "common/cfg/CFG.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/VariableTable.h"

using std::vector;

class SynonymUFDS {
 private:
  vector<int> parent;
  vector<uint16_t> rank;

 public:
  vector<PQLQuerySynonym*> synArr;
  explicit SynonymUFDS(VariableTable* varTable);

  void mergeSets(PQLQuerySynonym* syn1,
                 PQLQuerySynonym* syn2);
  bool isSameSet(PQLQuerySynonym* syn1,
                 PQLQuerySynonym* syn2);
  vector<int> getParents();
  PQLQuerySynonym* findSet(PQLQuerySynonym* syn);
  int indexOf(PQLQuerySynonym* syn);
};

