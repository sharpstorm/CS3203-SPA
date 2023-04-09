#pragma once
#include "common/Types.h"

class IFollowsWriter {
 public:
  virtual ~IFollowsWriter() {}
  virtual void addFollows(StmtValue arg1, StmtValue arg2) = 0;
};
