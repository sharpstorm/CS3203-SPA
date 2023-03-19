#pragma once

#include <unordered_set>
#include <utility>

#include "common/Types.h"
#include "IProvider.h"

using std::unordered_set;

class IEntityMappingProvider : public IProvider<EntityValue, EntityType> {
 public:
  virtual ~IEntityMappingProvider() {}
  virtual unordered_set<string> getValuesOfType(EntityType) const = 0;
  virtual string getVariableByIndex(int) const = 0;
  virtual string getConstantByIndex(int) const = 0;
  virtual unordered_set<int> getIndexOfVariable(string) const = 0;
  virtual unordered_set<int> getIndexOfConstant(string) const = 0;
};
