#include "QueryLauncher.h"

QueryLauncher::QueryLauncher(PkbQueryHandler pkbQH) : pkbQueryHandler(pkbQH) {
}

PQLQueryResult *QueryLauncher::execute(IEvaluatable *evaluatable) {
  return evaluatable->evaluateOn(pkbQueryHandler);
}
