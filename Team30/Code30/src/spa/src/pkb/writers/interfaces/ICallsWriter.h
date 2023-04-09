#pragma once

#include "common/Types.h"

class ICallsWriter {
 public:
  virtual ~ICallsWriter() {}
  virtual void addCalls(StmtValue arg1, EntityValue arg2, EntityValue arg3) = 0;
};
