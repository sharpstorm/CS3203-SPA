#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class NotEqualsASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_NOT_EQUALS;
  }
  string toString() {
    return ":neq";
  }
};
