#pragma once

#include "AbstractMathASTNode.h"

class DivASTNode: public AbstractMathASTNode {
 public:
    DivASTNode(): AbstractMathASTNode("/") {}
};
