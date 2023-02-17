#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class MinusASTNode: public BinaryASTNode {
 public:
  MinusASTNode() {
    type = ASTNODE_MINUS;
  }
  string toString() {
    return ":minus";
  }
};
