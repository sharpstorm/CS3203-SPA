#pragma once

#include "AbstractMathASTNode.h"

class PlusASTNode: public AbstractMathASTNode {
 public:
  PlusASTNode(): AbstractMathASTNode("+") {}
  int getPriority() {return 2;}
};
