#pragma once

#include <string>
#include "sp/common/ASTNode/ASTNode.h"
#include "StatementASTNode.h"

using std::string;

class AssignNode : public StatementASTNode {
 public:
  AssignNode();
  ~AssignNode() = default;
  string toString();
  void accept(shared_ptr<Extractor> e);
};
