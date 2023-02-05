#pragma once

#include <string>
#include <memory>
#include "ASTNode.h"

using std::string, std::shared_ptr;

class ReadNode : public ASTNode {
 public:
  ReadNode();
  ~ReadNode() = default;
  string toString();
  void accept();
  void addChild(shared_ptr<ASTNode> node);
  AST_NODE_TYPE getType() {return AST_NODE_READ;}
};
