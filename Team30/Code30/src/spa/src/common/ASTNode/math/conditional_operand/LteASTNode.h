#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class LteASTNode: public BinaryASTNode {
 public:
  LteASTNode() {
    type = ASTNODE_LTE;
  }
  string toString() {
    return ":lte";
  }
};
