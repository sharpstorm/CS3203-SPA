#pragma once

#include <string>
#include "common/PQLQueryResult.h"

class IQueryDriver {
 public:
  virtual PQLQueryResult* evaluate(string* query);
};
