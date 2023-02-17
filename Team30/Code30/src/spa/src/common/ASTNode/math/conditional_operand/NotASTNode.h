#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class NotASTNode: public BinaryASTNode {
 public:
  NotASTNode() : BinaryASTNode(ASTNODE_NOT, "") {
  }
  string toString() {
    return ":not";
  }
};
