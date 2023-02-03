#pragma once

#include <vector>
#include <memory>
#include "../common/PQLQuery.h"
#include "../common/IEvaluatable.h"

using std::vector;

class QueryPlanner {
 public:
  vector<shared_ptr<IEvaluatable>> getExecuteOrder(PQLQuery* query);
};
