#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "StorageTypes.h"
#include "common/Types.h"
#include "interfaces/IEntityMappingProvider.h"

using std::string;
using std::unordered_set;

class EntityMappingProvider : public IEntityMappingProvider {
 public:
  explicit EntityMappingProvider(VariableStorage *, ConstantStorage *,
                                 ProcedureStorage *);
  unordered_set<string> getValuesOfType(EntityType) const override;
  string getVariableByIndex(int) const override;
  string getConstantByIndex(int) const override;
  EntityIdx getIndexOfVariable(string) const override;
  EntityIdx getIndexOfConstant(string) const override;
  bool isValueOfType(EntityType, EntityValue) const override;

 private:
  VariableStorage *variableStorage;
  ConstantStorage *constantStorage;
  ProcedureStorage *procedureStorage;
};
