#pragma once
#include "AbstractConditionalNode.h"

class GteASTNode: public AbstractConditionalNode {
 public:
  GteASTNode() : AbstractConditionalNode(ASTNODE_GTE) {}
};
