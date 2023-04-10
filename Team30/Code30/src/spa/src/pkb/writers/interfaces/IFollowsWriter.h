#pragma once
#include "common/Types.h"

class IFollowsWriter {
 public:
  virtual ~IFollowsWriter() {}
  virtual void addFollows(StmtValue leftArg, StmtValue rightArg) = 0;
};
