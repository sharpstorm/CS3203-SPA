#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class DivASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_DIV;
  }
  string toString() {
    return ":div";
  }
};
