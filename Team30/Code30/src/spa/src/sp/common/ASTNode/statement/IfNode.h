#pragma once

#include <string>
#include <memory>
#include "sp/common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"

using std::string, std::shared_ptr;

class IfNode : public StatementASTNode {
 public:
  IfNode();
  ~IfNode() = default;
  string toString();
  void accept();
  void addChild(shared_ptr<ASTNode> node);
};
