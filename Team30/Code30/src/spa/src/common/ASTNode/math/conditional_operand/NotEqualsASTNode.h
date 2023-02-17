#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class NotEqualsASTNode: public BinaryASTNode {
 public:
  NotEqualsASTNode() : BinaryASTNode(ASTNODE_NOT_EQUALS, "") {
  }
  string toString() {
    return ":neq";
  }
};
