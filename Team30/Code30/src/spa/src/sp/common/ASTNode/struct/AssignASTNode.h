#pragma once

#include "sp/common/ASTNode/AbstractBinaryASTNode.h"

class AssignASTNode: public AbstractBinaryASTNode {
 public:
  void accept();
  string toString();
};
