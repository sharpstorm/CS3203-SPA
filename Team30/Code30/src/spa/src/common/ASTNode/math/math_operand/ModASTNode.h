#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class ModASTNode: public BinaryASTNode {
 public:
  ModASTNode() : BinaryASTNode(ASTNODE_MOD, "") {
  }
  string toString() {
    return ":mod";
  }
};
