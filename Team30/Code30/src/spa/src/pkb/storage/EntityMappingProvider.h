#pragma once

#include <vector>
#include <string>
#include <utility>
#include "SymbolStorage.h"
#include "../../common/Types.h"


class EntityMappingProvider {
 public:
    EntityMappingProvider();
    static SymbolStorage* symbolStorage;
    EntityType getSymbolTable(std::string);
    std::vector<std::string> getSymbolsOfType(EntityType);
};
