#pragma once

#include "common/Types.h"
#include "common/cfg/CFG.h"

class ICFGsQueryHandler {
 public:
  virtual ~ICFGsQueryHandler() {}
  virtual QueryResult<int, CFG*> queryCFGs(StmtRef) const = 0;
};
