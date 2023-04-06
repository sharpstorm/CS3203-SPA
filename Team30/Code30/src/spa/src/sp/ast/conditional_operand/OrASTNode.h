#pragma once

#include "AbstractConditionalNode.h"

class OrASTNode : public AbstractConditionalNode {
 public:
  OrASTNode() : AbstractConditionalNode(ASTNODE_OR) {}
};
