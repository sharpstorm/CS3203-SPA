#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class GteASTNode: public BinaryASTNode {
 public:
  GteASTNode() {
    type = ASTNODE_GTE;
  }
  string toString() {
    return ":gte";
  }
};
