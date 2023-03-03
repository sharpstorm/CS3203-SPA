#pragma once

#include <string>
#include <utility>

#include "../predicates/PredicateFactory.h"
#include "EntityMappingProvider.h"
#include "StorageTypes.h"
#include "StructureMappingProvider.h"
#include "common/Types.h"

class PKB {
 public:
  PKB();
  ~PKB();

  FollowsStorage *followsStore;
  ParentStorage *parentStore;
  SymbolStorage *symbolStorage;
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
  UsesStorage *usesStorage;
  UsesPStorage *usesPStorage;
  ModifiesStorage *modifiesStorage;
  ModifiesPStorage *modifiesPStorage;
  AssignStorage *assignStorage;
  CallsStorage *callsStorage;
  CallStmtStorage *callStmtStorage;
  StructureMappingProvider *structureProvider;
  EntityMappingProvider *entityMappingProvider;
  PredicateFactory *predicateFactory;
};
