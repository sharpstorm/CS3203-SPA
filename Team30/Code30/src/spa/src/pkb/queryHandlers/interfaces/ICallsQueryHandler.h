#pragma once

#include <string>

#include "../../../common/Types.h"

using std::string;

class ICallsQueryHandler {
 public:
  virtual ~ICallsQueryHandler() {}
  virtual QueryResult<string, string> queryCalls(EntityRef,
                                                 EntityRef) const = 0;
  virtual QueryResult<string, string> queryCallsStar(EntityRef,
                                                     EntityRef) const = 0;
};
