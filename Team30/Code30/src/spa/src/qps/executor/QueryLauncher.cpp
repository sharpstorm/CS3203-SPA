#include "QueryLauncher.h"

QueryLauncher::QueryLauncher(PkbQueryHandler* pkbQH): pkbQueryHandler(pkbQH) {
}

PQLQueryResult *QueryLauncher::execute(IEvaluatable *evaluatable,
                                       OverrideTable* table,
                                       QueryCache *cache) {
  QueryExecutorAgent agent(pkbQueryHandler, table, cache);
  return evaluatable->evaluateOn(agent);
}
