#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class LteASTNode: public AbstractConditionalNode {
 public:
  LteASTNode() : AbstractConditionalNode(ASTNODE_LTE, "") {
  }
  string toString() {
    return ":lte";
  }
};
