#pragma once

#include "common/ASTNode/ASTNode.h"

class ASTNode;
class IAssignWriter {
 public:
  virtual ~IAssignWriter() {}
  virtual void addAssigns(int, ASTNode) = 0;
};
