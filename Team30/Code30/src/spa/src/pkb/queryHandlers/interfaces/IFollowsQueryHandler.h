#pragma once

#include "../../../common/Types.h"

class IFollowsQueryHandler {
 public:
  virtual ~IFollowsQueryHandler() {}
  virtual QueryResult<int, int> queryFollows(StmtRef, StmtRef) const = 0;
  virtual QueryResult<int, int> queryFollowsStar(StmtRef, StmtRef) const = 0;
};
