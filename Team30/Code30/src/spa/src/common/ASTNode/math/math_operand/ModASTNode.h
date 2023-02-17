#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class ModASTNode: public BinaryASTNode {
 public:
  ModASTNode() {
    type = ASTNODE_MOD;
  }
  string toString() {
    return ":mod";
  }
};
