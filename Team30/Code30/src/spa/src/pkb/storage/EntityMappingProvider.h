#pragma once

#include <unordered_set>
#include <utility>
#include <string>
#include "SymbolStorage.h"
#include "../../common/Types.h"

class EntityMappingProvider {
 public:
  EntityMappingProvider();
  SymbolStorage *symbolStorage;
  EntityType getSymbolTable(std::string) const;
  std::unordered_set<std::string> getSymbolsOfType(EntityType) const;
};
