#pragma once

#include <memory>
#include <unordered_map>

#include "qps/common/PQLTypes.h"
#include "OverrideTransformer.h"

using std::unique_ptr, std::unordered_map;

class OverrideTable {
 public:
  bool contains(const PQLSynonymName &name);
  StmtRef transform(const PQLSynonymName &name, StmtRef ref);
  EntityRef transform(const PQLSynonymName &name, EntityRef ref);
  void insert(const PQLSynonymName &name,
              const OverrideTransformer &transformer);
  OverrideTransformer get(const PQLSynonymName &name);

 private:
  unordered_map<PQLSynonymName, OverrideTransformer> table;
};

typedef unique_ptr<OverrideTable> OverrideTablePtr;
