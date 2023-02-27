#pragma once

#include "AbstractMathNode.h"

class PlusASTNode: public AbstractMathNode {
 public:
  PlusASTNode() : AbstractMathNode(ASTNODE_PLUS, ":plus") {}
};
