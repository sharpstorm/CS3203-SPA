#pragma once

#include "common/Types.h"

class IModifiesWriter {
 public:
  virtual ~IModifiesWriter() {}
  /**
   * Adds relation where statement directly modifies variable.
   */
  virtual void addModifies(StmtValue stmtNum, EntityValue variable,
                           EntityValue procedure) = 0;
};
