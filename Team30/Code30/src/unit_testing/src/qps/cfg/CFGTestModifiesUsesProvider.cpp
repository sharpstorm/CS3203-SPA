#include "CFGTestModifiesUsesProvider.h"

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<EntityValue> modifies,
    vector<unordered_set<EntityValue>> uses):
    modifies(modifies), uses(uses) {}

    CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
        vector<EntityValue> modifies,
        vector<unordered_set<EntityValue>> uses,
        unordered_set<StmtValue> typeExclusions):
        modifies(modifies), uses(uses), typeExclusions(typeExclusions) {}

bool CFGTestModifiesUsesProvider::typePredicate(CFGTestModifiesUsesProvider *state,
                                                StmtType type,
                                                StmtValue value) {
  return state->typeExclusions.find(value) == state->typeExclusions.end();
}

EntityValue CFGTestModifiesUsesProvider::getModifies(CFGTestModifiesUsesProvider *state,
                                                     StmtValue value) {
  if (value < 1 || value > state->modifies.size()) {
    return "";
  }

  return state->modifies[value - 1];
}

unordered_set<EntityValue> CFGTestModifiesUsesProvider::getUses(
    CFGTestModifiesUsesProvider *state,
    StmtValue value) {
  if (value < 1 || value > state->uses.size()) {
    return {};
  }

  return state->uses[value - 1];
}