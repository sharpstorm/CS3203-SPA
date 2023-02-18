#pragma once

#include <string>
#include <memory>
#include "common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string, std::shared_ptr;

class ReadNode : public StatementASTNode {
 public:
  ReadNode(int lineNumber);
  ~ReadNode() = default;
  string toString();
  void accept(Extractor* e);
  void addChild(shared_ptr<ASTNode> node);
};
