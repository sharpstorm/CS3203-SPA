#pragma once

#include "qps/common/IntermediateResultSet.h"
#include "qps/common/IEvaluatable.h"

class QueryLauncher {
 public:
  IntermediateResultSet* execute(IEvaluatable* evaluatable);
};
