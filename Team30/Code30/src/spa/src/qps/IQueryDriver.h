#pragma once

#include <string>
#include "common/UtilityTypes.h"
#include "common/PQLQueryResult.h"

class IQueryDriver {
 public:
  virtual ~IQueryDriver() = default;
  virtual UniqueVectorPtr<string> evaluate(string* query) = 0;
};
