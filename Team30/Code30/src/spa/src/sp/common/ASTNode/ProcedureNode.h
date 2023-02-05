#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  ProcedureNode();
  ~ProcedureNode() = default;
  string toString();
  void accept();
  AST_NODE_TYPE getType() {return AST_NODE_PROCEDURE;}
};
