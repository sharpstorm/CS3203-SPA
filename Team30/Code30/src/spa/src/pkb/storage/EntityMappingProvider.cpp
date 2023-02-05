#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider() {}

EntityType EntityMappingProvider::getSymbolTable(std::string entityName) const {
  return symbolStorage->getSymbol(entityName);
}

std::unordered_set<std::string> EntityMappingProvider::getSymbolsOfType(
    EntityType entityType) const {
  return symbolStorage->getSymbolsOfType(entityType);
}
