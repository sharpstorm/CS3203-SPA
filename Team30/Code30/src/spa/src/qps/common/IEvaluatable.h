#pragma once

#include "IntermediateResultSet.h"

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual IntermediateResultSet* evaluateOn() = 0;
};
