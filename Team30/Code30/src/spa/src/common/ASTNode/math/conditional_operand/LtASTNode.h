#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class LtASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_LT;
  }
  string toString() {
    return ":lt";
  }
};
