#pragma once

#include "common/Types.h"

class IIfPatternQueryHandler {
 public:
  virtual ~IIfPatternQueryHandler() {}
  virtual QueryResult<StmtValue, EntityValue> queryIfPattern(StmtRef,
                                                             EntityRef) const = 0;
};
