#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "SymbolStorage.h"

class EntityMappingProvider {
 public:
  EntityMappingProvider();
  SymbolStorage *symbolStorage;
  EntityType getSymbolTable(std::string) const;
  std::unordered_set<std::string> getSymbolsOfType(EntityType) const;
};
