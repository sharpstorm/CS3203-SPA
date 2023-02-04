#pragma once

#include "AbstractMathASTNode.h"

class TimesASTNode: public AbstractMathASTNode {
 public:
  TimesASTNode(): AbstractMathASTNode("*") {}
    int getPriority() {return 1;}
};
