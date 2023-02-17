#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class AndASTNode: public BinaryASTNode {
 public:
  AndASTNode() : BinaryASTNode(ASTNODE_AND, "") {
  }
  string toString() {
    return ":and";
  }
};
