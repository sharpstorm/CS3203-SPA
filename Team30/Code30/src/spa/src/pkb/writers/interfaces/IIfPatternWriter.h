#pragma once

#include "common/Types.h"

class IIfPatternWriter {
 public:
  virtual ~IIfPatternWriter() {}
  virtual void addIfPattern(StmtValue stmt, EntityValue variable) = 0;
};
