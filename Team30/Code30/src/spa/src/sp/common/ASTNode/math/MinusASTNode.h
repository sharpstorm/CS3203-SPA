#pragma once

#include "AbstractMathASTNode.h"

class MinusASTNode: public AbstractMathASTNode {
 public:
    MinusASTNode(): AbstractMathASTNode("-") {}
    int getPriority() {return 2;}
};
