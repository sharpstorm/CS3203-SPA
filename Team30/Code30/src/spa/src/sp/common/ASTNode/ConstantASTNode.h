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
 private:
  string value;
};
