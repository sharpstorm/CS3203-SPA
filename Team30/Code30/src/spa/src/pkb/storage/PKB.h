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
  VariableStorage *variableStorage;
  ConstantStorage *constantStorage;
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
  UsesStorage *usesStorage;
  UsesPStorage *usesPStorage;
  ModifiesStorage *modifiesStorage;
  ModifiesPStorage *modifiesPStorage;
  AssignStorage *assignStorage;
  IfPatternStorage *ifPatternStorage;
  WhilePatternStorage *whilePatternStorage;
  CallsStorage *callsStorage;
  CallStmtStorage *callStmtStorage;
  StructureMappingProvider *structureProvider;
  EntityMappingProvider *entityMappingProvider;
  PredicateFactory *predicateFactory;
};
