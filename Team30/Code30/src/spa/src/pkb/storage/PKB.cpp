#include "PKB.h"

PKB::PKB()
    : symbolStorage(new SymbolStorage()),
      statementStorage(new StatementStorage()),
      procedureStorage(new ProcedureStorage()),
      structureProvider(new StructureMappingProvider()),
      entityMappingProvider(new EntityMappingProvider()) {}
