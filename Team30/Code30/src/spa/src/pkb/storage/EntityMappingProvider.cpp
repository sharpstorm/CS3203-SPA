#include <cassert>
#include "EntityMappingProvider.h"

EntityMappingProvider::EntityMappingProvider(SymbolStorage *symbolStorage)
    : symbolStorage(symbolStorage) {}

unordered_set<string> EntityMappingProvider::getSymbolsOfType(
    EntityType entityType) const {
  // note: EntityType::None is invalid
  return symbolStorage->get(entityType);
}
