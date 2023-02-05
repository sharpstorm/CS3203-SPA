#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class PrintNode : public ASTNode {
 public:
  PrintNode();
  ~PrintNode() = default;
  string toString();
  void accept();
  AST_NODE_TYPE getType() {return AST_NODE_PRINT;}
};
