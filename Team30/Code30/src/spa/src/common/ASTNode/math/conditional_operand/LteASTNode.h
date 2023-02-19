#pragma once
#include "AbstractConditionalNode.h"

class LteASTNode: public AbstractConditionalNode {
 public:
  LteASTNode() : AbstractConditionalNode(ASTNODE_LTE, ":lte") {}
};
