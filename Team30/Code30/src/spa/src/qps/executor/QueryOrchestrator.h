#pragma once

#include <vector>
#include "../common/PQLQuery.h"
#include "../common/QueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "QueryLauncher.h"

using std::vector;

class QueryOrchestrator {
    QueryLauncher launcher;
    ResultCoalescer coalescer;
 public:
  QueryResult* execute(PQLQuery* query, vector<IEvaluatable*> evaluatables);
};
