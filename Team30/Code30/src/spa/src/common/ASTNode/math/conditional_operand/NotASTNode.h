#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class NotASTNode: public AbstractConditionalNode {
 public:
  NotASTNode() : AbstractConditionalNode(ASTNODE_NOT, "") {
  }
  string toString() {
    return ":not";
  }
};
