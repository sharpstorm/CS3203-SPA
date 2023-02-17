#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class NotEqualsASTNode: public AbstractConditionalNode {
 public:
  NotEqualsASTNode() : AbstractConditionalNode(ASTNODE_NOT_EQUALS, "") {
  }
  string toString() {
    return ":neq";
  }
};
