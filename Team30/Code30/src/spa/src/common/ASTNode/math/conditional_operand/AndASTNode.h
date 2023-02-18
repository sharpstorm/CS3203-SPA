#pragma once
#include <string>
#include "common/ASTNode/BinaryASTNode.h"
#include "AbstractConditionalNode.h"

class AndASTNode: public AbstractConditionalNode {
 public:
  AndASTNode() : AbstractConditionalNode(ASTNODE_AND, "") {
  }
  string toString() {
    return ":and";
  }
};
