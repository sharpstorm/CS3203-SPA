#pragma once

#include <memory>

#include "common/Types.h"
#include "common/cfg/CFG.h"

class ICFGsWriter {
 public:
  virtual ~ICFGsWriter() {}
  virtual void addCFGs(EntityValue arg1, CFGSPtr arg2) = 0;
};
