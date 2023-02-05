#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class VariableASTNode : public ASTNode {
 public:
  explicit VariableASTNode(string value);
  ~VariableASTNode() = default;
  string toString();
  void accept();
  AST_NODE_TYPE getType() {return AST_NODE_VARIABLE;}
 private:
  string value;
};
