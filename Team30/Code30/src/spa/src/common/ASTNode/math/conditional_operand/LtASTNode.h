#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class LtASTNode: public AbstractConditionalNode {
 public:
  LtASTNode() : AbstractConditionalNode(ASTNODE_LT, "") {
  }
  string toString() {
    return ":lt";
  }
};
