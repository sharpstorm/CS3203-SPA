#pragma once

#include <vector>
#include "../common/PQLQuery.h"
#include "../common/IEvaluatable.h"

using std::vector;

class QueryPlanner {
 public:
  vector<IEvaluatable>* getExecuteOrder(PQLQuery* query);
};
