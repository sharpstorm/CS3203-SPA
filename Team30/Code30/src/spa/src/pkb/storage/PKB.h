#pragma once

#include "EntityMappingProvider.h"
#include "FollowsStorage.h"
#include "ParentStorage.h"
#include "ProcedureStorage.h"
#include "StatementStorage.h"
#include "StructureMappingProvider.h"
#include "SymbolStorage.h"

class PKB {
 public:
  PKB();
  FollowsStorage *followsStore;
  ParentStorage *parentStore;
  SymbolStorage *symbolStorage;
  StatementStorage *statementStorage;
  ProcedureStorage *procedureStorage;
  StructureMappingProvider *structureProvider;
  EntityMappingProvider *entityMappingProvider;
};
