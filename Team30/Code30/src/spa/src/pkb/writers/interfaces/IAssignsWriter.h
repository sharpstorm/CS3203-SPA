#pragma once

#include <memory>

#include "common/ast/IASTNode.h"

class IAssignsWriter {
 public:
  virtual ~IAssignsWriter() {}
  virtual void addAssigns(int arg1, shared_ptr<IASTNode> arg2) = 0;
};
