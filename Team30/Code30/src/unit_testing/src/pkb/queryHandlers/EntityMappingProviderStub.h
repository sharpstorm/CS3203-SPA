#pragma once

#include "common/Types.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/tables/ContiguousTable.h"
#include "pkb/storage/tables/HashKeySetTable.h"

class EntityMappingProviderStub : public IEntityMappingProvider {
 public:
  EntityValueSet allVariables;
  EntityValueSet allConstants;
  EntityValueSet allProcedures;

  ContiguousTable<EntityValue> variableTable;
  ContiguousTable<EntityValue> constantTable;
  ContiguousTable<EntityValue> procedureTable;

  HashKeyTable<EntityValue, EntityIdx> variableToIndexTable;
  HashKeyTable<EntityValue, EntityIdx> constantToIndexTable;
  HashKeyTable<EntityValue, EntityIdx> procedureToStmtNumTable;

  EntityMappingProviderStub();
  const EntityValueSet& getValuesOfType(EntityType) const override;
  bool isValueOfType(EntityType, EntityValue) const override;
  EntityValue getVariableByIndex(EntityIdx) const override;
  EntityValue getConstantByIndex(EntityIdx) const override;
  EntityIdx getIndexOfVariable(EntityValue) const override;
  EntityIdx getIndexOfConstant(EntityValue) const override;
};
