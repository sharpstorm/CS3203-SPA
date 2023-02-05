#pragma once

#include "PQLQueryResult.h"

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual PQLQueryResult* evaluateOn() = 0;
};
