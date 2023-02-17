#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class ModASTNode: public BinaryASTNode {
 public:ASTNodeType getType() {
    return ASTNODE_MOD;
  }
  string toString() {
    return ":mod";
  }
};
