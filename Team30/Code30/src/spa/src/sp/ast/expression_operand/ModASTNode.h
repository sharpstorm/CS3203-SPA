#pragma once

#include "AbstractExpressionNode.h"

class ModASTNode : public AbstractExpressionNode {
 public:
  ModASTNode() : AbstractExpressionNode(ASTNODE_MOD, "%") {}
};
