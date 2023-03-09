#pragma once

#include <string>
#include "common/Types.h"

using std::string;

class IWhilePatternQueryHandler {
 public:
  virtual ~IWhilePatternQueryHandler() {}
  virtual QueryResult<int, string> queryWhilePattern(EntityRef) const = 0;
};
