#pragma once

#include "FollowsStorage.h"
#include "ParentStorage.h"
#include "SymbolStorage.h"
#include "StatementStorage.h"
#include "ProcedureStorage.h"
#include "StructureMappingProvider.h"
#include "EntityMappingProvider.h"

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
