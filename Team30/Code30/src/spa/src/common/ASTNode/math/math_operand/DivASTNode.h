#pragma once

#include "AbstractMathNode.h"

class DivASTNode: public AbstractMathNode {
 public:
  DivASTNode() : AbstractMathNode(ASTNODE_DIV, ":div") {}
};
