#pragma once

#include <vector>

#include "common/Types.h"
#include "common/cfg/CFG.h"

using std::vector;

class ICFGsQueryHandler {
 public:
  virtual ~ICFGsQueryHandler() {}
  virtual vector<CFG*> queryCFGs(StmtRef) const = 0;
};
