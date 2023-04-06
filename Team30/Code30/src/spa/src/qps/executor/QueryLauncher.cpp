#include "QueryLauncher.h"

QueryLauncher::QueryLauncher(const PkbQueryHandler *pkbQH) :
    pkbQueryHandler(pkbQH) {}

PQLQueryResult *QueryLauncher::execute(const IEvaluatable *evaluatable,
                                       const OverrideTable *table,
                                       QueryCache *cache) const {
  QueryExecutorAgent agent(pkbQueryHandler, table, cache);
  return evaluatable->evaluateOn(agent);
}
