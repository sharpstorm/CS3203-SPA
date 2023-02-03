#pragma once

#include "QueryResult.h"

class IEvaluatable {
 public:
  virtual ~IEvaluatable() = default;
  virtual QueryResult* evaluateOn() = 0;
};
