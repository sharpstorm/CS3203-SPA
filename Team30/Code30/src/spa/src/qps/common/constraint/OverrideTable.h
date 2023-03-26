#pragma once

#include <memory>
#include <unordered_map>

#include "qps/common/PQLTypes.h"
#include "qps/constraints/OverrideTransformer.h"

using std::unique_ptr, std::unordered_map;

class OverrideTable {
 public:
  bool contains(const PQLSynonymName &name) const;
  StmtRef transformArg(const PQLSynonymName &name, StmtRef ref) const;
  EntityRef transformArg(const PQLSynonymName &name, EntityRef ref) const;
  void insert(const PQLSynonymName &name,
              const OverrideTransformer &transformer);
  OverrideTransformer get(const PQLSynonymName &name) const;

 private:
  unordered_map<PQLSynonymName, OverrideTransformer> table;
};

typedef unique_ptr<OverrideTable> OverrideTablePtr;
