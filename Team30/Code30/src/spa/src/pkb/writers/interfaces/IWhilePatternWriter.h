#pragma once

#include "common/Types.h"

class IWhilePatternWriter {
 public:
  virtual ~IWhilePatternWriter() {}
  virtual void addWhilePattern(StmtValue stmt, EntityValue variable) = 0;
};
