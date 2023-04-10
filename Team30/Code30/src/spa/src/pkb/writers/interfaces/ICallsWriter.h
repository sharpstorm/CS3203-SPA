#pragma once

#include "common/Types.h"

class ICallsWriter {
 public:
  virtual ~ICallsWriter() {}
  virtual void addCalls(StmtValue leftArg, EntityValue rightArg,
                        EntityValue arg3) = 0;
};
