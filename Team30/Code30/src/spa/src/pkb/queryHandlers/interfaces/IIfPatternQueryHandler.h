#pragma once

#include <string>
#include "common/Types.h"

using std::string;

class IIfPatternQueryHandler {
 public:
  virtual ~IIfPatternQueryHandler() {}
  virtual QueryResult<int, string> queryIfPattern(StmtRef, EntityRef) const = 0;
};
