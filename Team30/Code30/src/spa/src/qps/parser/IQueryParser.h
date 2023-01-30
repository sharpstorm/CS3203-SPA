#pragma once

#include <string>
#include "../common/PQLQuery.h"

using std::string;

class IQueryParser {
 public:
  virtual PQLQuery* parseQuery(string*);
};
