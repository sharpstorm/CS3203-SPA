#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class OrASTNode: public AbstractConditionalNode {
 public:
  OrASTNode() : AbstractConditionalNode(ASTNODE_OR, "") {
  }
  string toString() {
    return ":or";
  }
};
