#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"

class TimesASTNode: public BinaryASTNode {
 public:
  TimesASTNode() : BinaryASTNode(ASTNODE_TIMES, "") {
  }
  string toString() {
    return ":times";
  }
};
