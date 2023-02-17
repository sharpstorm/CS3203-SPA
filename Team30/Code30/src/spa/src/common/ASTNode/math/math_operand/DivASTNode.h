#pragma once
#include <string>
#include "AbstractMathNode.h"

class DivASTNode: public AbstractMathNode {
 public:
  DivASTNode() : AbstractMathNode(ASTNODE_DIV, "") {
  }
  string toString() {
    return ":div";
  }
};
