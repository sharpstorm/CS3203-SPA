#pragma once

#include <string>
#include "common/QueryResult.h"

class IQueryDriver {
 public:
  virtual QueryResult* evaluate(string* query);
};
