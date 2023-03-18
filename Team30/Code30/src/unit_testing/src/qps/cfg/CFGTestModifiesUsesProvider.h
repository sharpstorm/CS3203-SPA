#pragma once

#include <vector>
#include <unordered_set>
#include "common/Types.h"

using std::vector, std::unordered_set;

class CFGTestModifiesUsesProvider {
 public:
  CFGTestModifiesUsesProvider(vector<EntityValue> modifies,
                              vector<unordered_set<EntityValue>> uses);
  CFGTestModifiesUsesProvider(vector<EntityValue> modifies,
                              vector<unordered_set<EntityValue>> uses,
                              unordered_set<StmtValue> typeExclusions);

  static EntityValue getModifies(CFGTestModifiesUsesProvider* state,
                                          StmtValue value);
  static unordered_set<EntityValue> getUses(CFGTestModifiesUsesProvider* state,
                                                     StmtValue value);
  static bool typePredicate(CFGTestModifiesUsesProvider* state, StmtType type,
                            StmtValue value);

 private:
  vector<EntityValue> modifies;
  vector<unordered_set<EntityValue>> uses;
  unordered_set<StmtValue> typeExclusions;
};
