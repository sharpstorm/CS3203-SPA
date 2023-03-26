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

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<EntityValue> modifies,
    vector<unordered_set<EntityValue>> uses,
    int symbolCount, unordered_map<EntityValue, int> symbolTable,
    unordered_map<StmtValue, StmtType> typeExclusions) :
    modifies(modifies), uses(uses), symbolCount(symbolCount), symbolTable(symbolTable), typeExclusions(typeExclusions) {}

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

int CFGTestModifiesUsesProvider::getCount(const CFGTestModifiesUsesProvider &state) {
  return state.symbolCount;
}

int CFGTestModifiesUsesProvider::getSymbolId(const CFGTestModifiesUsesProvider &state,
                                             const EntityValue &value) {
  return state.symbolTable.at(value);
}