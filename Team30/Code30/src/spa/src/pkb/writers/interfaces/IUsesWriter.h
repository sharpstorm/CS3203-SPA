#pragma once

#include "common/Types.h"

class IUsesWriter {
 public:
  virtual ~IUsesWriter() {}
  /**
   * Adds relation where statement directly uses variable.
   */
  virtual void addUses(StmtValue stmtNum, EntityValue variable,
                       EntityValue procedure) = 0;
};
