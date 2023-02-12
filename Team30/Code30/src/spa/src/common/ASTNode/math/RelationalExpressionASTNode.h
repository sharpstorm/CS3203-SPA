#pragma once

#include "AbstractMathASTNode.h"

class RelationalExpressionASTNode: public AbstractMathASTNode {
 public:
    RelationalExpressionASTNode(): AbstractMathASTNode("re") {}
};
