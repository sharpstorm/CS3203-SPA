#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class AndASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_AND;
  }
  string toString() {
    return ":and";
  }
};
