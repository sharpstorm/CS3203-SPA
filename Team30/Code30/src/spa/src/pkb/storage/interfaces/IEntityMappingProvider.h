#pragma once

#include <set>
#include <utility>

#include "IProvider.h"
#include "common/Types.h"

using std::set;

class IEntityMappingProvider : public IProvider<EntityValue, EntityType> {
 public:
  virtual ~IEntityMappingProvider() {}
  virtual EntityValue getVariableByIndex(StmtValue) const = 0;
  virtual EntityValue getConstantByIndex(StmtValue) const = 0;
  virtual EntityIdx getIndexOfVariable(EntityValue) const = 0;
  virtual EntityIdx getIndexOfConstant(EntityValue) const = 0;
};
