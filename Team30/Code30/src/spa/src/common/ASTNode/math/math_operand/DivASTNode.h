#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class DivASTNode: public BinaryASTNode {
 public:
  DivASTNode() {
    type = ASTNODE_DIV;
  }
  string toString() {
    return ":div";
  }
};
