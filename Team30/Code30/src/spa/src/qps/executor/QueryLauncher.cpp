#include "QueryLauncher.h"

PQLQueryResult *QueryLauncher::execute(IEvaluatable *evaluatable) {
  return evaluatable->evaluateOn();
}
