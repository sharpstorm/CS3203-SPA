#pragma once

#include <vector>
#include <memory>
#include "../common/PQLQuery.h"
#include "../common/PQLQueryResult.h"
#include "../common/IEvaluatable.h"
#include "ResultCoalescer.h"
#include "QueryLauncher.h"

using std::vector;

class QueryOrchestrator {
    QueryLauncher launcher;
    ResultCoalescer coalescer;
 public:
  QueryOrchestrator(QueryLauncher launcher);
  PQLQueryResult* execute(PQLQuery* query,
                          vector<shared_ptr<IEvaluatable>> evaluatables);
};
