#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class AndASTNode: public BinaryASTNode {
 public:
  AndASTNode() {
    type = ASTNODE_AND;
  }
  string toString() {
    return ":and";
  }
};
