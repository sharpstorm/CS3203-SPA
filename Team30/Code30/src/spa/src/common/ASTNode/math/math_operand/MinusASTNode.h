#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class MinusASTNode: public BinaryASTNode {
 public:
  MinusASTNode() : BinaryASTNode(ASTNODE_MINUS, "") {
  }
  string toString() {
    return ":minus";
  }
};
