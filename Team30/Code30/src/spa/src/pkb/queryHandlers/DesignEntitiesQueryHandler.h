#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IDesignEntitiesQueryHandler.h"
#include "pkb/storage/EntityMappingProvider.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/StructureMappingProvider.h"

class DesignEntitiesQueryHandler : public IDesignEntitiesQueryHandler {
 public:
  DesignEntitiesQueryHandler(
      const IEntityMappingProvider *entityMappingProvider,
      const IStructureMappingProvider *structureMappingProvider);

  EntityValueSet getSymbolsOfType(EntityType) const override;
  StmtValueSet getStatementsOfType(StmtType) const override;
  StmtType getStatementType(StmtValue) const override;
  EntityValue getVariableByIndex(EntityIdx) const override;
  EntityValue getConstantByIndex(EntityIdx) const override;
  EntityIdx getIndexOfVariable(EntityValue) const override;
  EntityIdx getIndexOfConstant(EntityValue) const override;
  bool isStatementOfType(StmtType, StmtValue) const override;
  bool isSymbolOfType(EntityType, EntityValue) const override;
  EntityValue getCalledDeclaration(StmtValue) const override;

 private:
  const IEntityMappingProvider *entityMappingProvider;
  const IStructureMappingProvider *structureMappingProvider;
};
