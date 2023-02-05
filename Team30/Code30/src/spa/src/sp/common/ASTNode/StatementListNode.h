#pragma once

#include <string>
#include "ASTNode.h"

using std::string;

class StatementListNode : public ASTNode {
 public:
  StatementListNode();
  ~StatementListNode() = default;
  string toString();
  void accept();
};
