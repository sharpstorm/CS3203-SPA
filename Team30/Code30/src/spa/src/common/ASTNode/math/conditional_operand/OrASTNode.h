#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class OrASTNode: public BinaryASTNode {
 public:
  OrASTNode() : BinaryASTNode(ASTNODE_OR, ""){
  }
  string toString() {
    return ":or";
  }
};
