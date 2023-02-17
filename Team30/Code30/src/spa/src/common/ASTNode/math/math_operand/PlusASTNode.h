#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class PlusASTNode: public BinaryASTNode {
 public:
  PlusASTNode() : BinaryASTNode(ASTNODE_PLUS, "") {
  }
  string toString() {
    return ":plus";
  }
};
