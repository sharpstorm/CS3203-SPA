#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class LtASTNode: public BinaryASTNode {
 public:
  LtASTNode() {
    type = ASTNODE_LT;
  }
  string toString() {
    return ":lt";
  }
};
