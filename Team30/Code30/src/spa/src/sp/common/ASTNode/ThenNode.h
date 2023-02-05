#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class ThenNode : public ASTNode {
 public:
  ThenNode();
  ~ThenNode() = default;
  string toString();
  void accept();
  AST_NODE_TYPE getType() {return AST_NODE_THEN;}
};
