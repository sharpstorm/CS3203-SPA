#pragma once

#include <string>
#include "../common/QueryResult.h"

using std::string;

class ResultProjector {
 public:
  string* project(QueryResult*);
};
