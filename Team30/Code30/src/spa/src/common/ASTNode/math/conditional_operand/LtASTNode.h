#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class LtASTNode: public BinaryASTNode {
 public:
  LtASTNode() : BinaryASTNode(ASTNODE_LT, "") {
  }
  string toString() {
    return ":lt";
  }
};
