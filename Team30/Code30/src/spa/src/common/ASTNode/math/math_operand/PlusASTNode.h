#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class PlusASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_PLUS;
  }
  string toString() {
    return ":plus";
  }
};
