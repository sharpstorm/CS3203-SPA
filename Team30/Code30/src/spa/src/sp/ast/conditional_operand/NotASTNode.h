#pragma once
#include "AbstractConditionalNode.h"

class NotASTNode: public AbstractConditionalNode {
 public:
  NotASTNode() : AbstractConditionalNode(ASTNODE_NOT, ":not") {}
};
