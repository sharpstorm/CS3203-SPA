#pragma once

#include <memory>

#include "common/Types.h"
#include "common/pattern/PatternTrie.h"

class IAssignsWriter {
 public:
  virtual ~IAssignsWriter() {}
  virtual void addAssigns(StmtValue arg1, PatternTrieSPtr arg2) = 0;
};
