#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class DivASTNode: public BinaryASTNode {
 public:
  DivASTNode() : BinaryASTNode(ASTNODE_DIV, "") {
  }
  string toString() {
    return ":div";
  }
};
