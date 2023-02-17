#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class NotASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_NOT;
  }
  string toString() {
    return ":not";
  }
};
