#pragma once

#include <string>
#include <memory>
#include "sp/common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"

using std::string, std::shared_ptr;

class ReadNode : public StatementASTNode {
 public:
  ReadNode();
  ~ReadNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  void addChild(shared_ptr<ASTNode> node);
};
