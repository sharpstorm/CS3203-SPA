#pragma once

#include "IntermediateResultSet.h"

class IEvaluatable {
 public:
  virtual IntermediateResultSet* evaluateOn() = 0;
};
