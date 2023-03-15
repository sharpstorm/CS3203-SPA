#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "StorageTypes.h"
#include "interfaces/IEntityMappingProvider.h"

using std::string;
using std::unordered_set;

class EntityMappingProvider : public IEntityMappingProvider {
 public:
  explicit EntityMappingProvider(VariableStorage *, ConstantStorage *,
                                 ProcedureStorage *);
  unordered_set<string> getValuesOfType(EntityType) const override;
  bool isValueOfType(string, EntityType) const override;
  string getVariableByIndex(int) const override;
  string getConstantByIndex(int) const override;
  unordered_set<int> getIndexOfVariable(string) const override;
  unordered_set<int> getIndexOfConstant(string) const override;

 private:
  VariableStorage *variableStorage;
  ConstantStorage *constantStorage;
  ProcedureStorage *procedureStorage;
};
