#pragma once

#include "sp/ast/ASTNode.h"

class AST {
 public:
  explicit AST(ASTNodePtr node);
  ASTNodePtr getRoot();
 private:
  ASTNodePtr root;
};
