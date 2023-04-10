#pragma once

#include "common/Types.h"

class IParentWriter {
 public:
  virtual ~IParentWriter() {}
  virtual void addParent(StmtValue leftArg, StmtValue rightArg) = 0;
};
