#pragma once

#include "QueryPlanner.h"

vector<IEvaluatable>* QueryPlanner::getExecuteOrder(PQLQuery* query) {
  return new vector<IEvaluatable>();
}
