#pragma once

#include <string>
#include <memory>
#include "../common/PQLQuery.h"

using std::string, std::unique_ptr;

class IQueryParser {
 public:
  virtual ~IQueryParser() = default;
  virtual unique_ptr<PQLQuery> parseQuery(string*) = 0;
};
