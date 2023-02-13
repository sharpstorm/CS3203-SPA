#pragma once

#include <string>
#include "common/Types.h"

class IStatementWriter {
 public:
  virtual ~IStatementWriter() {}
  virtual void addStatement(int lineNumber, StmtType stmtType) = 0;

  /**
   * Add start and end statement numbers of a if or while statement.
   * @param start lineNum of if or while condition
   * @param end lineNum of last stmt in container block
   */
  virtual void addContainerStmt(int start, int end) = 0;
};
