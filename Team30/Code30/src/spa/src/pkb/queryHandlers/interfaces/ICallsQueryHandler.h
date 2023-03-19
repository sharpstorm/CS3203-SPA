#pragma once

#include "common/Types.h"

class ICallsQueryHandler {
 public:
  virtual ~ICallsQueryHandler() {}
  virtual QueryResult<EntityValue, EntityValue> queryCalls(EntityRef,
                                                           EntityRef) const = 0;
};
