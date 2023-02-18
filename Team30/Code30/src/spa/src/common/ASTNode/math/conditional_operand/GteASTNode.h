#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class GteASTNode: public AbstractConditionalNode {
 public:
  GteASTNode() : AbstractConditionalNode(ASTNODE_GTE, "") {
  }
  string toString() {
    return ":gte";
  }
};
