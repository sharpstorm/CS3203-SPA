#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "ProcedureAndCallsStorage.h"
#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IEntityMappingProvider.h"

using std::string;
using std::unordered_set;

class EntityMappingProvider : public IEntityMappingProvider {
 public:
  explicit EntityMappingProvider(VariableStorage *, ConstantStorage *,
                                 ProcedureAndCallsStorage *);
  const EntityValueSet &getValuesOfType(EntityType) const override;
  EntityValue getVariableByIndex(int) const override;
  EntityValue getConstantByIndex(int) const override;
  EntityIdx getIndexOfVariable(string) const override;
  EntityIdx getIndexOfConstant(string) const override;
  bool isValueOfType(EntityType, EntityValue) const override;

 private:
  VariableStorage *variableStorage;
  ConstantStorage *constantStorage;
  ProcedureAndCallsStorage *procedureStorage;
};
