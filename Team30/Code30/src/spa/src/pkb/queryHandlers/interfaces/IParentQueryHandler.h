#pragma once

#include "../../../common/Types.h"

class IParentQueryHandler {
  virtual QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const = 0;
  virtual QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2)
      const = 0;
};
