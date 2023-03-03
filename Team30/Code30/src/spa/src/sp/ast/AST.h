#pragma once

#include "sp/ast/ASTNode.h"
#include "common/ast/IAST.h"

class AST: public IAST {
 public:
  explicit AST(ASTNodePtr node);
  ASTNodePtr getMutableRoot();
  IASTNodePtr getRoot();
 private:
  ASTNodePtr root;
};
