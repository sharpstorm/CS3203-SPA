#pragma once

#include "AbstractMathASTNode.h"

class FactorASTNode: public AbstractMathASTNode {
 public:
    FactorASTNode(): AbstractMathASTNode("f") {}
};
