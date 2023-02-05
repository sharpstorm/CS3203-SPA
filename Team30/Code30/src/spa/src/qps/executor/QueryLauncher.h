#pragma once

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"

class QueryLauncher {
  PkbQueryHandler pkbQueryHandler;
 public:
  QueryLauncher(PkbQueryHandler pkbQH);
  PQLQueryResult* execute(IEvaluatable* evaluatable);
};
