#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class EqualsASTNode: public AbstractConditionalNode {
 public:
  EqualsASTNode() : AbstractConditionalNode(ASTNODE_EQUALS, "") {
  }
  string toString() {
    return ":eq";
  }
};
