#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class NotEqualsASTNode: public BinaryASTNode {
 public:
  NotEqualsASTNode() {
    type = ASTNODE_NOT_EQUALS;
  }
  string toString() {
    return ":neq";
  }
};
