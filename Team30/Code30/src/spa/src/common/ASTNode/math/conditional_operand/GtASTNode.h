#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class GtASTNode: public BinaryASTNode {
 public:
  GtASTNode() {
    type = ASTNODE_GT;
  }
  string toString() {
    return ":gt";
  }
};
