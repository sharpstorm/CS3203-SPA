#pragma once

#include <string>
#include "IQueryDriver.h"

using std::string;

class QueryDriver: public IQueryDriver {
 public:
  QueryResult* evaluate(string* query);
};
