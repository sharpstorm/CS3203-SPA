#pragma once

#include <memory>
#include "common/ast/ASTNode.h"

using std::shared_ptr;

class AST {
 public:
  explicit AST(shared_ptr<ASTNode> node);
  shared_ptr<ASTNode> getRoot();
 private:
  shared_ptr<ASTNode> root;
};
