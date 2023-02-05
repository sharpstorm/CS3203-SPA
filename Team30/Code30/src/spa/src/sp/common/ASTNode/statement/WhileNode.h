#pragma once

#include <string>
#include <memory>
#include "sp/common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"

using std::string, std::shared_ptr;

class WhileNode : public StatementASTNode {
 public:
  WhileNode();
  ~WhileNode() = default;
  string toString();
  void accept();
  void addChild(shared_ptr<ASTNode> node);
};
