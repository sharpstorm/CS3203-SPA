#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class NotASTNode: public BinaryASTNode {
 public:
  NotASTNode() {
    type = ASTNODE_NOT;
  }
  string toString() {
    return ":not";
  }
};
