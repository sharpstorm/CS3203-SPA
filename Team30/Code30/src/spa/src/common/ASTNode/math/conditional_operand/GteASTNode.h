#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class GteASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_GTE;
  }
  string toString() {
    return ":gte";
  }
};
