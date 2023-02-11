#pragma once

#include <string>
#include "common/Types.h"

class IStatementWriter {
 public:
  virtual ~IStatementWriter() {};
  virtual void addStatement(int, StmtType) = 0;
};
