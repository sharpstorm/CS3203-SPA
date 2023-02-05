#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class AssignNode : public ASTNode {
 public:
  AssignNode();
  ~AssignNode() = default;
  string toString();
  void accept();
  AST_NODE_TYPE getType() {return AST_NODE_ASSIGN;}
 private:
  string assignee;
};
