#pragma once

#include "AbstractConditionalNode.h"

class GtASTNode : public AbstractConditionalNode {
 public:
  GtASTNode() : AbstractConditionalNode(ASTNODE_GT) {}
};
