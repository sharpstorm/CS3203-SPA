#pragma once

#include "AbstractMathNode.h"

class MinusASTNode: public AbstractMathNode {
 public:
  MinusASTNode() : AbstractMathNode(ASTNODE_MINUS, ":minus") {}
};
