#pragma once

#include <string>
#include <unordered_set>

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/EntityMappingProvider.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"

using std::string;
using std::unordered_set;

class DesignEntitiesQueryHandler {
 public:
  DesignEntitiesQueryHandler(
      const IEntityMappingProvider *entityMappingProvider,
      const IStructureMappingProvider *structureMappingProvider);

  unordered_set<string> getSymbolsOfType(EntityType) const;
  unordered_set<int> getStatementsOfType(StmtType) const;

 private:
  const IEntityMappingProvider *entityMappingProvider;
  const IStructureMappingProvider *structureMappingProvider;
};
