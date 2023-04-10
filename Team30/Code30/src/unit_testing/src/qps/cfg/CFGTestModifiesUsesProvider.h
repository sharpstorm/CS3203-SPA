#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "common/Types.h"
#include "qps/common/CacheTable.h"
#include "qps/executor/QueryCache.h"

using std::vector, std::unordered_set, std::unordered_map;

class CFGTestModifiesUsesProvider {
 public:
  CFGTestModifiesUsesProvider(vector<unordered_set<EntityValue>> modifies,
                              vector<unordered_set<EntityValue>> uses,
                              QueryCache *cache);

  CFGTestModifiesUsesProvider(vector<unordered_set<EntityValue>> modifies,
                              vector<unordered_set<EntityValue>> uses,
                              unordered_map<StmtValue, StmtType> typeExclusions,
                              QueryCache *cache);

  static EntityIdxSet getModifies(const CFGTestModifiesUsesProvider &state,
                                  StmtValue value);
  static EntityIdxSet getUses(const CFGTestModifiesUsesProvider &state,
                              StmtValue value);

  static int getCount(const CFGTestModifiesUsesProvider &state);

  static bool typePredicate(const CFGTestModifiesUsesProvider &state,
                            StmtType type,
                            StmtValue value);

  CacheTable *getAffectsCache() const;

  EntityIdxSet getModifies(StmtValue value) const;
  EntityIdxSet getUses(StmtValue value) const;
  bool isStmtType(StmtType type,
                  StmtValue value) const;

  bool queryAffectsPartial(StmtValue arg0, StmtValue arg1) const;
  const CacheRow *queryAffectsFull(StmtValue arg0, StmtValue arg1) const;

  void addAffectsCache(StmtValue arg0, StmtValue arg1) {
    cache->getAffectsCache()->addEntry(arg0, arg1);
  }
  void promoteAffectsFrom(StmtValue from) {
    cache->getAffectsCache()->promoteFrom(from);
  }
  void promoteAffectsTo(StmtValue to) {
    cache->getAffectsCache()->promoteTo(to);
  }

 private:
  unordered_map<EntityValue, EntityIdx> symbolTable;
  vector<unordered_set<EntityValue>> modifies;
  vector<unordered_set<EntityValue>> uses;
  unordered_map<StmtValue, StmtType> typeExclusions;

  QueryCache *cache;

  void fillSymbolTable();
  static int getSymbolId(const CFGTestModifiesUsesProvider &state,
                         const EntityValue &value);
};
