#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider() {}

EntityType EntityMappingProvider::getSymbolTable(std::string entityName) {
    return symbolStorage->getSymbol(entityName);
}

std::vector<std::string> EntityMappingProvider::getSymbolsOfType \
    (EntityType entityType) {
    return symbolStorage->getSymbolsOfType(entityType);
}
