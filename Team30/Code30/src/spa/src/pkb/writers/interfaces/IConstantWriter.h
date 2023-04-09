#pragma once

#include "common/Types.h"

class IConstantWriter {
 public:
  virtual ~IConstantWriter() {}
  virtual EntityIdx addConstant(EntityValue) = 0;
};
