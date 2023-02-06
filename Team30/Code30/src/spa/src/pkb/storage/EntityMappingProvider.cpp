#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(SymbolStorage *symbolStorage)
    : symbolStorage(symbolStorage) {}

EntityType EntityMappingProvider::getSymbolTable(std::string entityName) const {
  return symbolStorage->getSymbol(entityName);
}

std::unordered_set<std::string> EntityMappingProvider::getSymbolsOfType(
    EntityType entityType) const {
  return symbolStorage->getSymbolsOfType(entityType);
}
