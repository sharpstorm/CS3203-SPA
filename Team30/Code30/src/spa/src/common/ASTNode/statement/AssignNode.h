#pragma once

#include <string>
#include <memory>
#include "StatementASTNode.h"
#include "common/ASTNode/ASTNode.h"
#include "sp/extractor/Extractor.h"

using std::string;

class AssignNode : public StatementASTNode {
 public:
  AssignNode();
  ~AssignNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
  ASTNodeType getType();
};
