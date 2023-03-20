#include "CFGTestModifiesUsesProvider.h"

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<EntityValue> modifies,
    vector<unordered_set<EntityValue>> uses):
    modifies(modifies), uses(uses) {}

    CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
        vector<EntityValue> modifies,
        vector<unordered_set<EntityValue>> uses,
        unordered_map<StmtValue, StmtType> typeExclusions):
        modifies(modifies), uses(uses), typeExclusions(typeExclusions) {}

bool CFGTestModifiesUsesProvider::typePredicate(const CFGTestModifiesUsesProvider &state,
                                                StmtType type,
                                                StmtValue value) {
  auto it = state.typeExclusions.find(value);
  if (it == state.typeExclusions.end()) {
    return type == StmtType::Assign;
  }
  return it->second == type;
}

EntityValue CFGTestModifiesUsesProvider::getModifies(const CFGTestModifiesUsesProvider &state,
                                                     StmtValue value) {
  if (value < 1 || value > state.modifies.size()) {
    return "";
  }

  return state.modifies[value - 1];
}

unordered_set<EntityValue> CFGTestModifiesUsesProvider::getUses(
    const CFGTestModifiesUsesProvider &state,
    StmtValue value) {
  if (value < 1 || value > state.uses.size()) {
    return {};
  }

  return state.uses[value - 1];
}