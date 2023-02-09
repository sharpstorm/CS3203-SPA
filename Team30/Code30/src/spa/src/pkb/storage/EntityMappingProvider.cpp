#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(SymbolStorage *symbolStorage)
    : symbolStorage(symbolStorage) {}

EntityType EntityMappingProvider::getTypeOfSymbol(string entityName) const {
  return symbolStorage->getSymbol(entityName);
}

unordered_set<string> EntityMappingProvider::getSymbolsOfType(
    EntityType entityType) const {
  return symbolStorage->getSymbolsOfType(entityType);
}

bool EntityMappingProvider::isSymbolOfType(string entityName,
                                           EntityType entityType) const {
  return entityType == symbolStorage->getSymbol(entityName);
}
