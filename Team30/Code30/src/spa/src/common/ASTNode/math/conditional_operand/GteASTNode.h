#pragma once
#include <string>
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
