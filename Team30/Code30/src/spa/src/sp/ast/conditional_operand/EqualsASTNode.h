#pragma once
#include "AbstractConditionalNode.h"

class EqualsASTNode: public AbstractConditionalNode {
 public:
  EqualsASTNode() : AbstractConditionalNode(ASTNODE_EQUALS) {}
};
