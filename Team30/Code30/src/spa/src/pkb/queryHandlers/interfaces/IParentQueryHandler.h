#pragma once

#include "common/Types.h"

class IParentQueryHandler {
 public:
  virtual ~IParentQueryHandler() {}
  virtual QueryResult<StmtValue, StmtValue> queryParent(StmtRef,
                                                        StmtRef) const = 0;
};
