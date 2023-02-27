#pragma once
#include "AbstractConditionalNode.h"

class NotEqualsASTNode: public AbstractConditionalNode {
 public:
  NotEqualsASTNode() : AbstractConditionalNode(ASTNODE_NOT_EQUALS, ":neq") {}
};
