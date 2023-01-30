#pragma once

#include "IntermediateResultSet.h"

class IEvaluatable {
 public:
  IntermediateResultSet* evaluateOn();
};
