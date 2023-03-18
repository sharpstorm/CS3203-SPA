#include "QueryLauncher.h"

QueryLauncher::QueryLauncher(PkbQueryHandler* pkbQH): pkbQueryHandler(pkbQH) {
}

PQLQueryResult *QueryLauncher::execute(IEvaluatable *evaluatable,
                                       OverrideTable* table) {
//  return evaluatable->evaluateOn(pkbQueryHandler, table);
  return evaluatable->evaluateOn(pkbQueryHandler);
}
