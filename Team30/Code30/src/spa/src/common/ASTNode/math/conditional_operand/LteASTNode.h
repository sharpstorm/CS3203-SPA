#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class LteASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_LTE;
  }
  string toString() {
    return ":lte";
  }
};
