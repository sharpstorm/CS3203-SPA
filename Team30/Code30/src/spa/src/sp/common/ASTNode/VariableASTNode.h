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
 private:
  string value;
};
