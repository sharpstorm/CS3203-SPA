#pragma once

#include "common/Types.h"

class IWhilePatternQueryHandler {
 public:
  virtual ~IWhilePatternQueryHandler() {}
  virtual QueryResult<StmtValue, EntityValue> queryWhilePattern(
      StmtRef, EntityRef) const = 0;
};
