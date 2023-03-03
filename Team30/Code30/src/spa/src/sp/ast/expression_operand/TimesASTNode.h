#pragma once

#include "AbstractExpressionNode.h"

class TimesASTNode: public AbstractExpressionNode {
 public:
  TimesASTNode() : AbstractExpressionNode(ASTNODE_TIMES, ":times") {}
};
