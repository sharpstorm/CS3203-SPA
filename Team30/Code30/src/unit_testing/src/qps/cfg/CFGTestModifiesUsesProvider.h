#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "common/Types.h"

using std::vector, std::unordered_set, std::unordered_map;

class CFGTestModifiesUsesProvider {
 public:
  CFGTestModifiesUsesProvider(vector<EntityValue> modifies,
                              vector<unordered_set<EntityValue>> uses);
  CFGTestModifiesUsesProvider(vector<EntityValue> modifies,
                              vector<unordered_set<EntityValue>> uses,
                              unordered_map<StmtValue, StmtType> typeExclusions);

  static EntityValue getModifies(const CFGTestModifiesUsesProvider &state,
                                          StmtValue value);
  static unordered_set<EntityValue> getUses(const CFGTestModifiesUsesProvider &state,
                                                     StmtValue value);
  static bool typePredicate(const CFGTestModifiesUsesProvider &state, StmtType type,
                            StmtValue value);

 private:
  vector<EntityValue> modifies;
  vector<unordered_set<EntityValue>> uses;
  unordered_map<StmtValue, StmtType> typeExclusions;
};
