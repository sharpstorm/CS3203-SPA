#pragma once
#include "AbstractConditionalNode.h"

class LtASTNode: public AbstractConditionalNode {
 public:
  LtASTNode() : AbstractConditionalNode(ASTNODE_LT, ":lt") {
  }
};
