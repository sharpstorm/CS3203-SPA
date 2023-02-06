#pragma once

#include <string>
#include <memory>
#include "sp/common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class IfNode : public StatementASTNode {
 public:
  IfNode();
  ~IfNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  void addChild(shared_ptr<ASTNode> node);
};
