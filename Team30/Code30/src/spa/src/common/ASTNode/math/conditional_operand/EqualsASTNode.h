#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class EqualsASTNode: public BinaryASTNode {
 public:
  EqualsASTNode() {
    type = ASTNODE_EQUALS;
  }
  string toString() {
    return ":eq";
  }
};
