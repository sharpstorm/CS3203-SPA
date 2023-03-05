#pragma once

#include <memory>

#include "common/pattern/PatternTrie.h"

class IAssignsWriter {
 public:
  virtual ~IAssignsWriter() {}
  virtual void addAssigns(int arg1, PatternTrieSPtr arg2) = 0;
};
