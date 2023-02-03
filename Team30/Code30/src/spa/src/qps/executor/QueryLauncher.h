#pragma once

#include "qps/common/QueryResult.h"
#include "qps/common/IEvaluatable.h"

class QueryLauncher {
 public:
  QueryResult* execute(IEvaluatable* evaluatable);
};
