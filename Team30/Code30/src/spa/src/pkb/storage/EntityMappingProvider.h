#pragma once

#include <utility>

#include "ProcedureAndCallsStorage.h"
#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IEntityMappingProvider.h"

class EntityMappingProvider : public IEntityMappingProvider {
 public:
  explicit EntityMappingProvider(VariableStorage *, ConstantStorage *,
                                 ProcedureAndCallsStorage *);
  const EntityValueSet &getValuesOfType(EntityType) const override;
  EntityValue getVariableByIndex(EntityIdx) const override;
  EntityValue getConstantByIndex(EntityIdx) const override;
  EntityIdx getIndexOfVariable(EntityValue) const override;
  EntityIdx getIndexOfConstant(EntityValue) const override;
  bool isValueOfType(EntityType, EntityValue) const override;

 private:
  VariableStorage *variableStorage;
  ConstantStorage *constantStorage;
  ProcedureAndCallsStorage *procedureStorage;
};
