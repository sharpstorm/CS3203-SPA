#pragma once

#include "AbstractMathASTNode.h"

class ConditionalExpressionASTNode: public AbstractMathASTNode {
 public:
  ConditionalExpressionASTNode(): AbstractMathASTNode("ce") {}
};
