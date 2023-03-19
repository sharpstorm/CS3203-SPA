#pragma once

#include "common/Types.h"

class ICallsTQueryHandler {
 public:
  virtual ~ICallsTQueryHandler() {}
  virtual QueryResult<EntityValue, EntityValue> queryCallsStar(
      EntityRef, EntityRef) const = 0;
};
