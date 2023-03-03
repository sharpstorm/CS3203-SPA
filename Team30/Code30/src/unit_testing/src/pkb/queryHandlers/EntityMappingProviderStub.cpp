#include "EntityMappingProviderStub.h"

EntityMappingProviderStub::EntityMappingProviderStub() {};

unordered_set<string> EntityMappingProviderStub::getSymbolsOfType(EntityType type) const {
  return entityTypeToValue.get(type);
}
