#pragma once

#include "../predicates/PredicateFactory.h"
#include "EntityMappingProvider.h"
#include "ProcedureStorage.h"
#include "StatementStorage.h"
#include "StorageTypes.h"
#include "StructureMappingProvider.h"
#include "SymbolStorage.h"

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
