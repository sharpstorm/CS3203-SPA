#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class WhileNode : public StatementASTNode {
 public:
  WhileNode(int lineNumber);
  ~WhileNode() = default;
  string toString();
  void accept(Extractor* e);
  void leave(Extractor* e);
  void addChild(shared_ptr<ASTNode> node);
};
