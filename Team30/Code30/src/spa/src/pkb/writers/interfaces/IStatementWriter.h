#pragma once

#include "common/Types.h"

class IStatementWriter {
 public:
  virtual ~IStatementWriter() {}
  virtual void addStatement(StmtValue lineNumber, StmtType stmtType) = 0;
};
