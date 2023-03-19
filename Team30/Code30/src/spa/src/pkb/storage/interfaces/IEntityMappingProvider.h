#pragma once

#include <unordered_set>
#include <utility>

#include "common/Types.h"
#include "IProvider.h"

using std::unordered_set;

class IEntityMappingProvider : public IProvider<EntityValue, EntityType> {
 public:
  virtual ~IEntityMappingProvider() {}
  virtual EntityValue getVariableByIndex(StmtValue) const = 0;
  virtual EntityValue getConstantByIndex(StmtValue) const = 0;
  virtual unordered_set<StmtValue> getIndexOfVariable(EntityValue) const = 0;
  virtual unordered_set<StmtValue> getIndexOfConstant(EntityValue) const = 0;
};
