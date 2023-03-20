#include "QueryLauncher.h"

QueryLauncher::QueryLauncher(PkbQueryHandler* pkbQH): pkbQueryHandler(pkbQH) {
}

PQLQueryResult *QueryLauncher::execute(IEvaluatable *evaluatable,
                                       OverrideTable* table) {
  QueryExecutorAgent agent(pkbQueryHandler, table);
  return evaluatable->evaluateOn(agent);
}
