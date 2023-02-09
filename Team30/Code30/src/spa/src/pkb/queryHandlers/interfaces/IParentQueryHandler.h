#pragma once

#include "../../../common/Types.h"

class IParentQueryHandler {
  virtual QueryResult<int, int> queryParent(StmtRef, StmtRef) const = 0;
  virtual QueryResult<int, int> queryParentStar(StmtRef, StmtRef)
  const = 0;
};
