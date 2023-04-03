#pragma once

#include <vector>
#include <unordered_map>

#include "common/cfg/CFG.h"
#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/VariableTable.h"

using std::vector, std::unordered_map;

typedef int UFDSSet;
typedef uint16_t UFDSRank;

class SynonymUFDS {
 private:
  vector<UFDSSet> parent;
  vector<UFDSRank> rank;
  unordered_map<PQLSynonymName, UFDSSet> synNameMap;
  vector<PQLQuerySynonym *> synArr;

 public:
  explicit SynonymUFDS(VariableTable *varTable);


  bool tryMergeSets(const PQLSynonymName &syn1,
                    const PQLSynonymName &syn2);
  bool isSameSet(const PQLSynonymName &syn1,
                 const PQLSynonymName &syn2);
  vector<UFDSSet> getParents() const;
  PQLQuerySynonym *getSynFor(const PQLSynonymName &set);

 private:
  UFDSSet findSet(const UFDSSet &set);
  UFDSSet indexOf(const PQLSynonymName &syn) const;
  bool mergeSyns(const UFDSSet &setA, const UFDSSet &setB);
};
