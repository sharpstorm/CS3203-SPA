#pragma once

#include "AbstractExpressionNode.h"

class DivASTNode : public AbstractExpressionNode {
 public:
  DivASTNode() : AbstractExpressionNode(ASTNODE_DIV, "/") {}
};
