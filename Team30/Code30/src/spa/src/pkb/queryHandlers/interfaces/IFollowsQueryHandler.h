#pragma once

#include "../../../common/Types.h"

class IFollowsQueryHandler {
  virtual QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const = 0;
};
