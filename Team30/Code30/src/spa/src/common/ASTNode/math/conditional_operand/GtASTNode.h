#pragma once
#include <string>
#include "AbstractConditionalNode.h"

class GtASTNode: public AbstractConditionalNode {
 public:
  GtASTNode() : AbstractConditionalNode(ASTNODE_GT, "") {
  }
  string toString() {
    return ":gt";
  }
};
