#pragma once
#include <string>
#include <vector>
#include "ASTNode.h"

using std::string, std::vector;

class ConstantASTNode : public ASTNode {
 public:
  ConstantASTNode(string value);
  ~ConstantASTNode() = default;
  string toString();
  void accept();

 private:
  string value;
};
