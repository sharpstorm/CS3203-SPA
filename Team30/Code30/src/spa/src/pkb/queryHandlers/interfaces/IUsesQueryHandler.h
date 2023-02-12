#pragma once

#include <string>
#include "common/Types.h"

class IUsesQueryHandler {
 public:
  virtual ~IUsesQueryHandler() {}
  virtual QueryResult<int, string> queryUses(StmtRef, EntityRef) const = 0;
  virtual QueryResult<string, string> queryUses(EntityRef, EntityRef) const = 0;
};
