#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "common/Types.h"

class SymbolStorage {
 public:
    SymbolStorage();
    void addSymbol(const std::string&, EntityType);
    EntityType getSymbol(const std::string&);
    std::vector<std::string> getSymbolsOfType(EntityType);
 private:
    std::unordered_map<std::string, EntityType> SymbolTable;
    std::unordered_map<EntityType, std::vector<std::string>> ReverseSymbolTable;
};
