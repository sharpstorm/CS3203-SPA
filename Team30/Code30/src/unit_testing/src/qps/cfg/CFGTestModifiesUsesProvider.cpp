#include "CFGTestModifiesUsesProvider.h"

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<EntityValue> modifies,
    vector<unordered_set<EntityValue>> uses):
    modifies(modifies), uses(uses) {
  fillSymbolTable();
}

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<EntityValue> modifies,
    vector<unordered_set<EntityValue>> uses,
    unordered_map<StmtValue, StmtType> typeExclusions):
    modifies(modifies), uses(uses), typeExclusions(typeExclusions) {
  fillSymbolTable();
}

bool CFGTestModifiesUsesProvider::typePredicate(const CFGTestModifiesUsesProvider &state,
                                                StmtType type,
                                                StmtValue value) {
  auto it = state.typeExclusions.find(value);
  if (it == state.typeExclusions.end()) {
    return type == StmtType::Assign;
  }
  return it->second == type;
}

EntityIdx CFGTestModifiesUsesProvider::getModifies(const CFGTestModifiesUsesProvider &state,
                                                   StmtValue value) {
  if (value < 1 || value > state.modifies.size()) {
    return NO_ENT_INDEX;
  }

  auto entity = state.modifies[value - 1];
  return getSymbolId(state, entity);
}

EntityIdxSet CFGTestModifiesUsesProvider::getUses(
    const CFGTestModifiesUsesProvider &state,
    StmtValue value) {
  if (value < 1 || value > state.uses.size()) {
    return {};
  }

  auto entSet = state.uses[value - 1];
  EntityIdxSet ret;
  for (auto item : entSet) {
    ret.insert(getSymbolId(state, item));
  }
  ret.erase(NO_ENT_INDEX);

  return ret;
}

int CFGTestModifiesUsesProvider::getCount(const CFGTestModifiesUsesProvider &state) {
  return state.symbolTable.size() + 1;
}

EntityIdx CFGTestModifiesUsesProvider::getSymbolId(const CFGTestModifiesUsesProvider &state,
                                                   const EntityValue &value) {
  if (value == NO_ENT) {
    return NO_ENT_INDEX;
  }
  if (state.symbolTable.find(value) == state.symbolTable.end()) {
    return NO_ENT_INDEX;
  }
  return state.symbolTable.at(value);
}

void CFGTestModifiesUsesProvider::fillSymbolTable() {
  int counter = 1;

  for (auto x : modifies) {
    if (x == NO_ENT) {
      continue;
    }

    if (symbolTable.find(x) == symbolTable.end()) {
      symbolTable.emplace(x, counter);
      counter++;
    }
  }

  for (auto x : uses) {
    for (auto y : x) {
      if (y == NO_ENT) {
        continue;
      }
      if (symbolTable.find(y) == symbolTable.end()) {
        symbolTable.emplace(y, counter);
        counter++;
      }
    }
  }
}
