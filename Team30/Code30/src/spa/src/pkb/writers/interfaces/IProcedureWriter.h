#pragma once

#include "common/Types.h"

class IProcedureWriter {
 public:
  virtual ~IProcedureWriter() {}
  virtual void addProcedure(EntityValue, StmtValue, StmtValue) = 0;
};
