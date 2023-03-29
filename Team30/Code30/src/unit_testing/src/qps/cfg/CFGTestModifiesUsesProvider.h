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

  static EntityIdx getModifies(const CFGTestModifiesUsesProvider &state,
                               StmtValue value);
  static EntityIdxSet getUses(const CFGTestModifiesUsesProvider &state,
                              StmtValue value);

  static int getCount(const CFGTestModifiesUsesProvider &state);

  static bool typePredicate(const CFGTestModifiesUsesProvider &state,
                            StmtType type,
                            StmtValue value);

 private:
  unordered_map<EntityValue, EntityIdx> symbolTable;
  vector<EntityValue> modifies;
  vector<unordered_set<EntityValue>> uses;
  unordered_map<StmtValue, StmtType> typeExclusions;

  void fillSymbolTable();
  static int getSymbolId(const CFGTestModifiesUsesProvider &state,
                         const EntityValue &value);
};
