#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class MinusASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_MINUS;
  }
  string toString() {
    return ":minus";
  }
};
