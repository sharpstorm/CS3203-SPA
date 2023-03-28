#pragma once

#include "common/Types.h"
#include "pkb/queryHandlers/BaseQueryHandler.h"

class IFollowsQueryHandler {
 public:
  virtual ~IFollowsQueryHandler() = default;

  virtual QueryResultPtr<StmtValue, StmtValue> query(StmtRef*,
                                                     StmtRef*) const = 0;
};
