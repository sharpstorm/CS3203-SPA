#pragma once

#include <unordered_set>
#include <string>
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/tables/HashKeySetTable.h"
#include "pkb/storage/tables/HashKeyTable.h"

using std::unordered_set;
using std::string;

class EntityMappingProviderStub : public IEntityMappingProvider {
 public:
  HashKeySetTable<EntityType, string> entityTypeToValue;

  EntityMappingProviderStub();
  unordered_set<string> getSymbolsOfType(EntityType) const override;
};
