#pragma once

#include "AbstractExpressionNode.h"

class PlusASTNode: public AbstractExpressionNode {
 public:
  PlusASTNode() : AbstractExpressionNode(ASTNODE_PLUS, ":plus") {}
};
