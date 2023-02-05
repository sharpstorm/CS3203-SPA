#pragma once

#include "AbstractMathASTNode.h"

class TimesASTNode: public AbstractMathASTNode {
 public:
  TimesASTNode(): AbstractMathASTNode("*") {}
};
