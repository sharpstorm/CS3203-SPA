#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class GtASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_GT;
  }
  string toString() {
    return ":gt";
  }
};
