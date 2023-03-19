#pragma once

#include "common/Types.h"

class IFollowsQueryHandler {
 public:
  virtual ~IFollowsQueryHandler() {}
  virtual QueryResult<StmtValue, StmtValue> queryFollows(StmtRef,
                                                         StmtRef) const = 0;
};
