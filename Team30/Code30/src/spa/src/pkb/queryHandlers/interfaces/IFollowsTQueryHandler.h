#pragma once

#include "common/Types.h"

class IFollowsTQueryHandler {
 public:
  virtual ~IFollowsTQueryHandler() {}
  virtual QueryResult<StmtValue, StmtValue> queryFollowsStar(StmtRef,
                                                             StmtRef) const = 0;
};

