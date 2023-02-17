#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class OrASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_OR;
  }
  string toString() {
    return ":or";
  }
};
