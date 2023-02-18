#pragma once
#include <string>
#include "AbstractMathNode.h"

class TimesASTNode: public AbstractMathNode {
 public:
  TimesASTNode() : AbstractMathNode(ASTNODE_TIMES, ":times") {}
};
