#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class ProcedureNode : public ASTNode {
 public:
  ProcedureNode(string name);
  ~ProcedureNode() = default;
  string toString();
  void accept();

 private:
  string procName;
};
