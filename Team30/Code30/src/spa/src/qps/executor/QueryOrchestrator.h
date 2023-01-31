#pragma once

#include <vector>
#include "../common/PQLQuery.h"
#include "../common/QueryResult.h"
#include "../common/IEvaluatable.h"

using std::vector;

class QueryOrchestrator {
 public:
  QueryResult* execute(PQLQuery* query, vector<IEvaluatable>* evaluatables);
};
