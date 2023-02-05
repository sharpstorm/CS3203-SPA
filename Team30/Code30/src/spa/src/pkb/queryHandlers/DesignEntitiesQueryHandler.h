#pragma once

#include <string>
#include <unordered_set>

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/EntityMappingProvider.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"

class DesignEntitiesQueryHandler {
 public:
  DesignEntitiesQueryHandler(
      const EntityMappingProvider* entityMappingProvider,
      const StructureMappingProvider* structureMappingProvider);

  std::unordered_set<std::string> getSymbolsOfType(EntityType) const;
  std::unordered_set<int> getStatementsOfType(StmtType) const;

 private:
  const EntityMappingProvider* entityMappingProvider;
  const StructureMappingProvider* structureMappingProvider;
};
