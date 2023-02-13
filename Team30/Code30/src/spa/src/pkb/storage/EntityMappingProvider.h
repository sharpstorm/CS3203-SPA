#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "SymbolStorage.h"
#include "interfaces/IEntityMappingProvider.h"

using std::string;
using std::unordered_set;

class EntityMappingProvider : public IEntityMappingProvider {
 public:
  explicit EntityMappingProvider(SymbolStorage *);
  EntityType getTypeOfSymbol(string) const override;
  unordered_set<string> getSymbolsOfType(EntityType) const override;
  bool isSymbolOfType(string, EntityType) const override;

 private:
  SymbolStorage *symbolStorage;
};
