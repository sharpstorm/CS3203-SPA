#include "QueryLauncher.h"

QueryLauncher::QueryLauncher(shared_ptr<PkbQueryHandler> pkbQH) {
  pkbQueryHandler = pkbQH;
}

PQLQueryResult *QueryLauncher::execute(IEvaluatable *evaluatable) {
  return evaluatable->evaluateOn(pkbQueryHandler);
}
