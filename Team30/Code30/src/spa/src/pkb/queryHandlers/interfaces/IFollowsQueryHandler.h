#pragma once

#include "../../../common/Types.h"

class IFollowsQueryHandler {
 public:
  virtual QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const = 0;
  virtual QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2)
      const = 0;
};
