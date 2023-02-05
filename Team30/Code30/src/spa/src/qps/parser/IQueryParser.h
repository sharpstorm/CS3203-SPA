#pragma once

#include <string>
#include <memory>
#include "../common/PQLQuery.h"

using std::string, std::unique_ptr;

class IQueryParser {
 public:
  virtual unique_ptr<PQLQuery> parseQuery(string*);
};
