#pragma once

#include "AbstractMathNode.h"

class ModASTNode: public AbstractMathNode {
 public:
  ModASTNode() : AbstractMathNode(ASTNODE_MOD, ":mod") {}
};
