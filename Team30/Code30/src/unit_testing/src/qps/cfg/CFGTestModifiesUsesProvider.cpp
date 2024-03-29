#include "CFGTestModifiesUsesProvider.h"

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<unordered_set<EntityValue>> modifies,
    vector<unordered_set<EntityValue>> uses,
    QueryCache *cache) :
    modifies(modifies), uses(uses), cache(cache) {
  fillSymbolTable();
}

CFGTestModifiesUsesProvider::CFGTestModifiesUsesProvider(
    vector<unordered_set<EntityValue>> modifies,
    vector<unordered_set<EntityValue>> uses,
    unordered_map<StmtValue, StmtType> typeExclusions,
    QueryCache *cache) :
    modifies(modifies),
    uses(uses),
    typeExclusions(typeExclusions),
    cache(cache) {
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

EntityIdxSet CFGTestModifiesUsesProvider::getModifies(const CFGTestModifiesUsesProvider &state,
                                                      StmtValue value) {
  if (value < 1 || value > state.modifies.size()) {
    return {};
  }

  auto entSet = state.modifies[value - 1];
  EntityIdxSet ret;
  for (auto item : entSet) {
    ret.insert(getSymbolId(state, item));
  }
  ret.erase(NO_ENT_INDEX);

  return ret;
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

CacheTable *CFGTestModifiesUsesProvider::getAffectsCache() const {
  return cache->getAffectsCache();
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

EntityIdxSet CFGTestModifiesUsesProvider::getUses(StmtValue value) const {
  return getUses(*this, value);
}

EntityIdxSet CFGTestModifiesUsesProvider::getModifies(StmtValue value) const {
  return getModifies(*this, value);
}

bool CFGTestModifiesUsesProvider::isStmtType(StmtType type,
                                             StmtValue value) const {
  return typePredicate(*this, type, value);
}

bool CFGTestModifiesUsesProvider::queryAffectsPartial(StmtValue arg0,
                                                      StmtValue arg1) const {
  return false;
}

const CacheRow *CFGTestModifiesUsesProvider::queryAffectsFull(StmtValue arg0,
                                                              StmtValue arg1) const {
  return nullptr;
}