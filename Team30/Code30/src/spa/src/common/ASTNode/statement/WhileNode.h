#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class WhileNode : public StatementASTNode {
 public:
  WhileNode();
  ~WhileNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  void addChild(shared_ptr<ASTNode> node);
};
