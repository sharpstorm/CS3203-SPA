#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(SymbolStorage *symbolStorage)
    : symbolStorage(symbolStorage) {}

EntityType EntityMappingProvider::getTypeOfSymbol(string entityName) const {
  return symbolStorage->getByKey(entityName);
}

unordered_set<string> EntityMappingProvider::getSymbolsOfType(
    EntityType entityType) const {
  if (entityType == EntityType::None) {
    return symbolStorage->getAllKeys();
  }
  return symbolStorage->getByValue(entityType);
}
