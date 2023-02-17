#include "EntityMappingProviderStub.h"

EntityMappingProviderStub::EntityMappingProviderStub() {};

EntityType EntityMappingProviderStub::getTypeOfSymbol(string symbol) const {
  return valueToEntityType.get(symbol);
}

unordered_set<string> EntityMappingProviderStub::getSymbolsOfType(EntityType type) const {
  if (type == EntityType::None) {
    return allSymbols;
  }
  return entityTypeToValue.get(type);
}
