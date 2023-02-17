#pragma once

#include <string>
#include "common/Types.h"

class IStatementWriter {
 public:
  virtual ~IStatementWriter() {}
  virtual void addStatement(int lineNumber, StmtType stmtType) = 0;
  // // todo: remove
  virtual void addContainerStmt(int start, int end) = 0;
};
