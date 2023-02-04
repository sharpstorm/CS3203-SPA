#pragma once

#include "AbstractMathASTNode.h"

class RelationalExpressionASTNode: public AbstractMathASTNode {
 public:
    RelationalExpressionASTNode(string repr): AbstractMathASTNode(repr) {}
    int getPriority() {return 0;}
};
