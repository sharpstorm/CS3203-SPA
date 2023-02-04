#pragma once

#include "AbstractMathASTNode.h"

class ModASTNode: public AbstractMathASTNode {
 public:
    ModASTNode(): AbstractMathASTNode("%") {}
    int getPriority() {return 1;}
};
