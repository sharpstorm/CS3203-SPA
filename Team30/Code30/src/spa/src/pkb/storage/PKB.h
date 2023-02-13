#pragma once

#include <string>
#include "../predicates/PredicateFactory.h"
#include "EntityMappingProvider.h"
#include "ProcedureStorage.h"
#include "StorageTypes.h"
#include "StructureMappingProvider.h"
#include "common/Types.h"

class PKB {
 public:
  PKB();
  ~PKB();
  FollowsStorage* followsStore;
  ParentStorage* parentStore;
  SymbolStorage* symbolStorage;
  StatementStorage* statementStorage;
  ProcedureStorage* procedureStorage;
  StructureMappingProvider* structureProvider;
  EntityMappingProvider* entityMappingProvider;
  PredicateFactory* predicateFactory;
};
