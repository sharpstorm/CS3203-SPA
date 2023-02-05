#pragma once
#include <string>
#include "ASTNode.h"

using std::string, std::vector;

class ConstantASTNode : public ASTNode {
 public:
  explicit ConstantASTNode(string value);
  ~ConstantASTNode() = default;
  string toString();
  void accept();
  AST_NODE_TYPE getType() {return AST_NODE_CONST;}
 private:
  string value;
};
