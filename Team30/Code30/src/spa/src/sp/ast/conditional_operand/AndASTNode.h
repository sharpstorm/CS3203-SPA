#pragma once
#include "AbstractConditionalNode.h"

class AndASTNode: public AbstractConditionalNode {
 public:
  AndASTNode() : AbstractConditionalNode(ASTNODE_AND, ":and") {}
};
