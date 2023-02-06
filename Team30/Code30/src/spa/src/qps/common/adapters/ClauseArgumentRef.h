#pragma once

#include "common/Types.h"
#include "qps/clauses/ClauseArgument.h"

class ClauseArgumentRef {
 public:
  static StmtRef toStmtRef(ClauseArgument clauseArgument);
  static EntityRef toEntityRef(ClauseArgument clauseArgument);
};
