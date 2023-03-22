#pragma once

#include <string>
#include "../common/PQLQuery.h"

using std::string;

class IQueryParser {
 public:
  virtual ~IQueryParser() = default;
  virtual PQLQueryPtr parseQuery(string*) = 0;
};
