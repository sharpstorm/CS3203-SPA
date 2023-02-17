#pragma once

#include "common/ASTNode/BinaryASTNode.h"

class TimesASTNode: public BinaryASTNode {
 public:
  TimesASTNode() {
    type = ASTNODE_TIMES;
  }
  string toString() {
    return ":times";
  }
};
