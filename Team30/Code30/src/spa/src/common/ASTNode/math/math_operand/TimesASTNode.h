#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class TimesASTNode: public BinaryASTNode {
 public:
  ASTNodeType getType() {
    return ASTNODE_TIMES;
  }
  string toString() {
    return ":times";
  }
};
