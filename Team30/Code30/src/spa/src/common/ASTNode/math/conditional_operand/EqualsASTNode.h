#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class EqualsASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_EQUALS;
  }
  string toString() {
    return ":eq";
  }
};
