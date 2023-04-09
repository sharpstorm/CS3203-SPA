#pragma once

#include "common/Types.h"

class IParentWriter {
 public:
  virtual ~IParentWriter() {}
  virtual void addParent(StmtValue arg1, StmtValue arg2) = 0;
};
