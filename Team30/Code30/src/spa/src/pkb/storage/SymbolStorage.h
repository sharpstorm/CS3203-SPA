#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common/Types.h"

class SymbolStorage {
 public:
  SymbolStorage();
  void addSymbol(const std::string &, EntityType);
  EntityType getSymbol(const std::string &) const;
  std::unordered_set<std::string> getSymbolsOfType(EntityType) const;
 private:
  std::unordered_map<std::string, EntityType> SymbolTable;
  std::unordered_map<EntityType, std::unordered_set<std::string>>
      ReverseSymbolTable;
};
