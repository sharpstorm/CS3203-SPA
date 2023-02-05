#pragma once

#include <string>
#include "StatementASTNode.h"

using std::string;

class PrintNode : public StatementASTNode {
 public:
  PrintNode();
  ~PrintNode() = default;
  string toString();
  void accept();
};
