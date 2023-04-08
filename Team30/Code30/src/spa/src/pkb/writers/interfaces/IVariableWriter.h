#pragma once

#include "common/Types.h"

class IVariableWriter {
 public:
  virtual ~IVariableWriter() {}
  virtual EntityIdx addVariable(EntityValue) = 0;
};
