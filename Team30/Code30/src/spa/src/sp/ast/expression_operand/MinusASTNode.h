#pragma once

#include "AbstractExpressionNode.h"

class MinusASTNode: public AbstractExpressionNode {
 public:
  MinusASTNode() : AbstractExpressionNode(ASTNODE_MINUS) {}
};
