#pragma once

#include <string>
#include "common/PQLQueryResult.h"

class IQueryDriver {
 public:
  virtual ~IQueryDriver() = default;
  virtual PQLQueryResult* evaluate(string* query) = 0;
};
