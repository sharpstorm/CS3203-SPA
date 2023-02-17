#pragma once

#include <string>
#include <memory>
#include "StatementASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class PrintNode : public StatementASTNode {
 public:
  PrintNode();
  ~PrintNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  ASTNodeType getType();
};
