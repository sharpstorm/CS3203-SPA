#pragma once

#include <memory>

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/common/IEvaluatable.h"

using std::shared_ptr;

class QueryLauncher {
  shared_ptr<PkbQueryHandler> pkbQueryHandler;
 public:
  explicit QueryLauncher(shared_ptr<PkbQueryHandler> pkbQH);
  PQLQueryResult* execute(IEvaluatable* evaluatable);
};
