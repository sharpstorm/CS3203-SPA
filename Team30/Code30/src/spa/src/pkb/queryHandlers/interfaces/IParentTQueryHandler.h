#pragma once

#include "common/Types.h"

class IParentTQueryHandler {
 public:
  virtual ~IParentTQueryHandler() {}
  virtual QueryResult<StmtValue, StmtValue> queryParentStar(StmtRef,
                                                            StmtRef) const = 0;
};
